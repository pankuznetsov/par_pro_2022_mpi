// Copyright 2022 Grachev Valentin
#include "../../../modules/task_3/grachev_v_mark_components/mark_components.h"

Image::Image(int _width, int _height) {
    width = _width;
    height = _height;
    pixels.reserve(width * height);
    for (int i = 0; i < width * height; i++)
        pixels.push_back(0);
}

Image::Image(const Image &other) : pixels(other.pixels) {
    width = other.width;
    height = other.height;
}

int Image::GetWidth() const { return width; }

int Image::GetHeight() const { return height; }

void Image::FillRandomBinary() {
    std::random_device dev;
    std::mt19937 gen(dev());

    for (int i = 0; i < width * height; i += 1)
        pixels[i] = gen() % 2;
}

int Image::GetPixel(int x, int y) const { return pixels[y * width + x]; }

void Image::SetPixel(int x, int y, int value) { pixels[y * width + x] = value; }

bool Image::PixelExists(int x, int y) const {
    if (0 <= x && x < width && 0 <= y && y < height)
        return true;
    return false;
}

int *Image::ToDataMPI() const {
    int *data = new int[width * height + 2];
    data[0] = width;
    data[1] = height;
    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++)
            data[2 + y * width + x] = GetPixel(x, y);

    return data;
}

void Image::Show() const {
    for (int i = 0; i < width * height; i++) {
        if (i % width == 0)
            cout << "\n";
        cout << pixels[i] << "\t";
    }
}

Image Image::GetImageStrings(int fromY_inclusive, int toY_exclusive) const {
    Image res(width, toY_exclusive - fromY_inclusive);
    for (int y = 0; y < res.GetHeight(); y++)
        for (int x = 0; x < res.GetWidth(); x++)
            res.SetPixel(x, y, GetPixel(x, fromY_inclusive + y));
    return res;
}

bool Image::operator==(const Image &other) const {
    if (width != other.width || height != other.height)
        return false;

    for (int i = 0; i < width * height; i++)
        if (pixels[i] != other.pixels[i])
            return false;

    return true;
}

Image GetPreset1() {
    Image image(4, 4);
    image.SetPixel(0, 0, 1);
    image.SetPixel(1, 0, 0);
    image.SetPixel(2, 0, 1);
    image.SetPixel(3, 0, 0);
    image.SetPixel(0, 1, 1);
    image.SetPixel(1, 1, 0);
    image.SetPixel(2, 1, 0);
    image.SetPixel(3, 1, 1);
    image.SetPixel(0, 2, 1);
    image.SetPixel(1, 2, 0);
    image.SetPixel(2, 2, 1);
    image.SetPixel(3, 2, 0);
    image.SetPixel(0, 3, 1);
    image.SetPixel(1, 3, 1);
    image.SetPixel(2, 3, 1);
    image.SetPixel(3, 3, 1);
    return image;
}

void MoveCoordVector(vector<Coord> *to, vector<Coord> *from) {
    for (int i = 0; i < from->size(); i++)
        to->push_back(from->operator[](i));
    from->clear();
}

Image PreMarking(const Image &source, int *last_mark, int start_mark) {
    int current_mark = start_mark;
    Image premarked_image(source);

    for (int y = 0; y < source.GetHeight(); y++) {
        for (int x = 0; x < source.GetWidth(); x++) {
            // marked left pixel
            if (source.PixelExists(x - 1, y) &&
                source.GetPixel(x - 1, y) == 1 && source.GetPixel(x, y) == 1) {
                premarked_image.SetPixel(x, y,
                                         premarked_image.GetPixel(x - 1, y));
            } else if (source.PixelExists(x, y - 1) &&
                       source.GetPixel(x, y - 1) == 1 &&
                       source.GetPixel(x, y) == 1) {
                premarked_image.SetPixel(x, y,
                                         premarked_image.GetPixel(x, y - 1));
            } else if (source.GetPixel(x, y) == 1) {
                premarked_image.SetPixel(x, y, current_mark);
                current_mark++;
            }
        }
    }

    *last_mark = current_mark;
    return premarked_image;
}

Image PostMarking(const Image &premarked_image, int last_mark) {
    vector<vector<Coord>>
        marked_groups;  // marked_groups[i] - get vector with pixel coords which
                        // have mark number i;
    marked_groups.reserve(last_mark + 1);
    for (int i = 0; i <= last_mark; i++) {
        vector<Coord> empty;
        marked_groups.push_back(empty);
    }

    set<MarkPair> mark_pairs_for_merge;
    vector<MarkPair> unique_mark_pairs_for_merge;

    for (int x = 0; x < premarked_image.GetWidth(); x++) {
        for (int y = 0; y < premarked_image.GetHeight(); y++) {
            // Make mark groups
            if (premarked_image.GetPixel(x, y) != 0) {
                int pixel_mark = premarked_image.GetPixel(x, y);
                Coord coord;
                coord.x = x;
                coord.y = y;
                marked_groups[pixel_mark].push_back(coord);
            }

            // Make mark pairs for merge
            if (premarked_image.PixelExists(x, y - 1) &&
                premarked_image.GetPixel(x, y - 1) != 0 &&
                premarked_image.GetPixel(x, y) != 0 &&
                premarked_image.GetPixel(x, y) !=
                    premarked_image.GetPixel(x, y - 1)) {
                MarkPair pair;
                pair.lower_mark = premarked_image.GetPixel(x, y);
                pair.upper_mark = premarked_image.GetPixel(x, y - 1);
                mark_pairs_for_merge.insert(pair);
            }
        }
    }

    for (auto pair : mark_pairs_for_merge)
        unique_mark_pairs_for_merge.push_back(pair);

    for (int i = 0; i < unique_mark_pairs_for_merge.size(); i++) {
        int to_mark = unique_mark_pairs_for_merge[i].upper_mark;
        int from_mark = unique_mark_pairs_for_merge[i].lower_mark;
        if (to_mark != from_mark)
            MoveCoordVector(&marked_groups[to_mark], &marked_groups[from_mark]);
        // Update pairs
        for (int j = i + 1; j < unique_mark_pairs_for_merge.size(); j++) {
            if (unique_mark_pairs_for_merge[j].lower_mark == from_mark)
                unique_mark_pairs_for_merge[j].lower_mark = to_mark;

            else if (unique_mark_pairs_for_merge[j].upper_mark == from_mark)
                unique_mark_pairs_for_merge[j].upper_mark = to_mark;
        }
    }

    int finish_mark = 0;
    Image finish_marked_image(premarked_image);
    for (int i = 0; i < marked_groups.size(); i++) {
        if (!marked_groups[i].empty())
            finish_mark++;
        for (int j = 0; j < marked_groups[i].size(); j++) {
            finish_marked_image.SetPixel(marked_groups[i][j].x,
                                         marked_groups[i][j].y, finish_mark);
        }
    }
    return finish_marked_image;
}

Image MarkingSequential(const Image &source, double *time) {
    int last_mark;
    Image marked_image = PreMarking(source, &last_mark, 1);

    Image result = PostMarking(marked_image, last_mark);
    return result;
}

Image MarkingParallel(const Image &source, double *time) {
    int rank, proc_count;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_count);

    if (rank == 0) {  // Main process
        int str_quant_for_process = source.GetHeight() / proc_count;
        int str_quant_for_main_process =
            str_quant_for_process + source.GetHeight() % proc_count;

        // Make data for send
        for (int i = 1; i < proc_count; i++) {
            int process_start_y =
                str_quant_for_main_process + str_quant_for_process * (i - 1);

            int size = str_quant_for_process * source.GetWidth() + 2;
            int size_for_send = size;
            int mark_for_send =
                (str_quant_for_main_process + str_quant_for_process * (i - 1)) *
                    source.GetWidth() / 2 +
                1;
            Image image_for_send = source.GetImageStrings(
                process_start_y, process_start_y + str_quant_for_process);
            int *data_for_send = image_for_send.ToDataMPI();

            MPI_Send(&size_for_send, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&mark_for_send, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
            MPI_Send(data_for_send, size, MPI_INT, i, 0, MPI_COMM_WORLD);
        }

        // Main process handle own part
        int last_main_mark;
        Image image_main =
            source.GetImageStrings(0, str_quant_for_main_process);
        Image marked_image_main = PreMarking(image_main, &last_main_mark, 1);

        // Collect all data and put them to vector
        vector<Image> premarked_parts;
        premarked_parts.push_back(marked_image_main);
        for (int i = 1; i < proc_count; i++) {
            int size = str_quant_for_process * source.GetWidth() + 2;
            int *accepted_data = new int[size];
            MPI_Recv(accepted_data, size, MPI_INT, i, 0, MPI_COMM_WORLD,
                     MPI_STATUSES_IGNORE);
            Image builded_image_part = GetImageFromDataMPI(accepted_data);
            premarked_parts.push_back(builded_image_part);
        }

        Image premarked_finish_image(source.GetWidth(), source.GetHeight());
        int current_y = 0;
        // Build premarked finish image
        for (int i = 0; i < premarked_parts.size(); i++) {
            for (int y = 0; y < premarked_parts[i].GetHeight();
                 y++, current_y++) {
                for (int x = 0; x < premarked_parts[i].GetWidth(); x++)
                    premarked_finish_image.SetPixel(
                        x, current_y, premarked_parts[i].GetPixel(x, y));
            }
        }
        int last_mark = source.GetHeight() * source.GetWidth() / 2 + 1;

        return PostMarking(premarked_finish_image, last_mark);

    } else {  // Other process
        int size, start_mark, dum;
        MPI_Recv(&size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
        MPI_Recv(&start_mark, 1, MPI_INT, 0, 1, MPI_COMM_WORLD,
                 MPI_STATUSES_IGNORE);

        int *image_data = new int[size];
        MPI_Recv(image_data, size, MPI_INT, 0, 0, MPI_COMM_WORLD,
                 MPI_STATUSES_IGNORE);

        Image image_part = GetImageFromDataMPI(image_data);
        Image premarked_image_part = PreMarking(image_part, &dum, start_mark);

        int *data_for_send = premarked_image_part.ToDataMPI();
        MPI_Send(data_for_send, size, MPI_INT, 0, 0, MPI_COMM_WORLD);

        return Image(1, 1);
    }
}

Image GetImageFromDataMPI(int *data_mpi) {
    int width = data_mpi[0];
    int height = data_mpi[1];
    Image result(width, height);
    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++)
            result.SetPixel(x, y, data_mpi[2 + y * width + x]);
    return result;
}

bool CheckMarking(const Image &marked_image) {
    for (int y = 0; y < marked_image.GetHeight(); y++) {
        for (int x = 0; x < marked_image.GetWidth(); x++) {
            if (marked_image.GetPixel(x, y) == 0)
                continue;

            // Up
            if (marked_image.PixelExists(x, y - 1) &&
                marked_image.GetPixel(x, y - 1) != 0 &&
                marked_image.GetPixel(x, y) != marked_image.GetPixel(x, y - 1))
                return false;

            // Down
            if (marked_image.PixelExists(x, y + 1) &&
                marked_image.GetPixel(x, y + 1) != 0 &&
                marked_image.GetPixel(x, y) != marked_image.GetPixel(x, y + 1))
                return false;

            // Left
            if (marked_image.PixelExists(x - 1, y) &&
                marked_image.GetPixel(x - 1, y) != 0 &&
                marked_image.GetPixel(x, y) != marked_image.GetPixel(x - 1, y))
                return false;

            // Right
            if (marked_image.PixelExists(x + 1, y) &&
                marked_image.GetPixel(x + 1, y) != 0 &&
                marked_image.GetPixel(x, y) != marked_image.GetPixel(x + 1, y))
                return false;
        }
    }
    return true;
}
