// Copyright 2022 Chernyshev Nikita
#include "../../../modules/task_3/chernyshev_n_sobel_filter/sobel_filter.h"

ByteImage::ByteImage(int _width, int _height) {
    pixels.reserve(_width * _height);
    height = _height;
    width = _width;
    for (int i = 0; i < height * width; i++)
        pixels.push_back(0);
}

ByteImage::ByteImage(const ByteImage &image) : pixels(image.pixels) {
    width = image.width;
    height = image.height;
}

int ByteImage::Width() const { return width; }

int ByteImage::Height() const { return height; }

void ByteImage::Randomize() {
    std::random_device dev;
    std::mt19937 gen(dev());

    for (int i = 0; i < width * height; i++)
        pixels[i] = (unsigned char)(gen() % 256);
}

unsigned char ByteImage::GetPixel(int x, int y) const {
    if (x < 0)
        x = 0;
    else if (x >= width)
        x = width - 1;

    if (y < 0)
        y = 0;
    else if (y >= height)
        y = height - 1;

    return pixels[y * width + x];
}

void ByteImage::SetPixel(int x, int y, unsigned char value) {
    pixels[y * width + x] = value;
}

void ByteImage::Print() {
    for (int i = 0; i < Width() * Height(); i++) {
        if (i % Width() == 0)
            cout << "\n";
        cout << static_cast<int>(pixels[i]) << "\t";
    }
}

unsigned char *ByteImage::GetPixelsRows(int from_y_inclusive,
                                        int to_y_exclusive) const {
    unsigned char *data =
        new unsigned char[width * (to_y_exclusive - from_y_inclusive)];
    int i = 0;
    for (int y = from_y_inclusive; y < to_y_exclusive; y++, i++)
        for (int x = 0; x < width; x++)
            data[i * width + x] = GetPixel(x, y);
    return data;
}

bool ByteImage::operator==(const ByteImage &image) const {
    if (pixels == image.pixels)
        return true;
    return false;
}

unsigned char SobelPixelHandle(vector<unsigned char> around_pixels) {
    int Gy[3][3];
    Gy[0][0] = -1;
    Gy[1][0] = -2;
    Gy[2][0] = -1;
    Gy[0][1] = 0;
    Gy[1][1] = 0;
    Gy[2][1] = 0;
    Gy[0][2] = 1;
    Gy[1][2] = 2;
    Gy[2][2] = 1;
    int Gx[3][3];
    Gx[0][0] = -1;
    Gx[1][0] = 0;
    Gx[2][0] = 1;
    Gx[0][1] = -2;
    Gx[1][1] = 0;
    Gx[2][1] = 2;
    Gx[0][2] = -1;
    Gx[1][2] = 0;
    Gx[2][2] = 1;

    int res_Gx = 0, res_Gy = 0;

    res_Gx += Gx[0][0] * around_pixels[0];
    res_Gx += Gx[1][0] * around_pixels[1];
    res_Gx += Gx[2][0] * around_pixels[2];
    res_Gx += Gx[0][1] * around_pixels[3];
    res_Gx += Gx[1][1] * around_pixels[4];
    res_Gx += Gx[2][1] * around_pixels[5];
    res_Gx += Gx[0][2] * around_pixels[6];
    res_Gx += Gx[1][2] * around_pixels[7];
    res_Gx += Gx[2][2] * around_pixels[8];

    res_Gy += Gy[0][0] * around_pixels[0];
    res_Gy += Gy[1][0] * around_pixels[1];
    res_Gy += Gy[2][0] * around_pixels[2];
    res_Gy += Gy[0][1] * around_pixels[3];
    res_Gy += Gy[1][1] * around_pixels[4];
    res_Gy += Gy[2][1] * around_pixels[5];
    res_Gy += Gy[0][2] * around_pixels[6];
    res_Gy += Gy[1][2] * around_pixels[7];
    res_Gy += Gy[2][2] * around_pixels[8];

    int res = sqrt(pow(res_Gx, 2) + pow(res_Gy, 2));
    if (res > 255)
        res = 255;
    else if (res < 0)
        res = 0;

    return (unsigned char)res;
}

ByteImage GetCustomImage() {
    ByteImage image(5, 5);
    image.SetPixel(0, 0, 0);
    image.SetPixel(1, 0, 0);
    image.SetPixel(2, 0, 0);
    image.SetPixel(3, 0, 0);
    image.SetPixel(4, 0, 255);
    image.SetPixel(0, 1, 0);
    image.SetPixel(1, 1, 0);
    image.SetPixel(2, 1, 0);
    image.SetPixel(3, 1, 255);
    image.SetPixel(4, 1, 0);
    image.SetPixel(0, 2, 0);
    image.SetPixel(1, 2, 0);
    image.SetPixel(2, 2, 255);
    image.SetPixel(3, 2, 0);
    image.SetPixel(4, 2, 0);
    image.SetPixel(0, 3, 0);
    image.SetPixel(1, 3, 255);
    image.SetPixel(2, 3, 0);
    image.SetPixel(3, 3, 0);
    image.SetPixel(4, 3, 0);
    image.SetPixel(0, 4, 255);
    image.SetPixel(1, 4, 0);
    image.SetPixel(2, 4, 0);
    image.SetPixel(3, 4, 0);
    image.SetPixel(4, 4, 0);

    return image;
}

ByteImage SobelSequential(const ByteImage &origin) {
    ByteImage res(origin);

    for (int y = 0; y < origin.Height(); y++) {
        for (int x = 0; x < origin.Width(); x++) {
            vector<unsigned char> around_pixels;

            for (int i = -1; i <= 1; i++)
                for (int j = -1; j <= 1; j++)
                    around_pixels.push_back(origin.GetPixel(x + j, y + i));

            unsigned char res_pixel = SobelPixelHandle(around_pixels);
            res.SetPixel(x, y, res_pixel);
        }
    }

    return res;
}

ByteImage SobelParallel(const ByteImage &origin) {
    int rank, proc_quant;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_quant);
    ByteImage result(origin);

    if (rank == 0) {
        int rows_for_process = origin.Height() / proc_quant;
        int rows_for_main_process =
            rows_for_process + origin.Height() % proc_quant;

        for (int i = 1; i < proc_quant; i++) {
            int from_y = rows_for_main_process + rows_for_process * (i - 1) - 1;
            int to_y = rows_for_main_process + rows_for_process * i + 1;

            unsigned char *data = origin.GetPixelsRows(from_y, to_y);

            int width = origin.Width();
            int height = rows_for_process + 2;

            MPI_Send(&width, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&height, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
            MPI_Send(data, width * height, MPI_UNSIGNED_CHAR, i, 0,
                     MPI_COMM_WORLD);
        }

        unsigned char *data =
            origin.GetPixelsRows(0, rows_for_main_process + 1);
        ByteImage image_part(origin.Width(), rows_for_main_process + 1);
        for (int y = 0; y < rows_for_main_process + 1; y++)
            for (int x = 0; x < origin.Width(); x++)
                image_part.SetPixel(x, y, origin.GetPixel(x, y));

        ByteImage handled_image_part = SobelSequential(image_part);
        for (int y = 0; y < handled_image_part.Height() - 1; y++)
            for (int x = 0; x < handled_image_part.Width(); x++)
                result.SetPixel(x, y, handled_image_part.GetPixel(x, y));

        for (int i = 1; i < proc_quant; i++) {
            int size = rows_for_process * origin.Width();
            unsigned char *accepted_data = new unsigned char[size];
            MPI_Recv(accepted_data, size, MPI_UNSIGNED_CHAR, i, 0,
                     MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
            int start_y = rows_for_main_process + (i - 1) * rows_for_process;

            for (int y = 0; y < rows_for_process; y++)
                for (int x = 0; x < result.Width(); x++)
                    result.SetPixel(x, start_y + y,
                                    accepted_data[y * origin.Width() + x]);
        }

        return result;

    } else {
        int width, height;
        MPI_Recv(&width, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
        MPI_Recv(&height, 1, MPI_INT, 0, 1, MPI_COMM_WORLD,
                 MPI_STATUSES_IGNORE);
        unsigned char *data = new unsigned char[width * height];
        MPI_Recv(data, width * height, MPI_UNSIGNED_CHAR, 0, 0, MPI_COMM_WORLD,
                 MPI_STATUSES_IGNORE);

        ByteImage image(width, height);
        for (int y = 0; y < height; y++)
            for (int x = 0; x < width; x++)
                image.SetPixel(x, y, data[y * width + x]);
        ByteImage handled_image = SobelSequential(image);

        unsigned char *data_for_send =
            handled_image.GetPixelsRows(1, handled_image.Height() - 1);
        MPI_Send(data_for_send, width * (height - 2), MPI_UNSIGNED_CHAR, 0, 0,
                 MPI_COMM_WORLD);

        return ByteImage(1, 1);
    }
}
