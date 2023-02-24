// Copyright 2022 Musin Alexandr
#include "../../../modules/task_3/musin_a_global_opt_character_parr/musin_a_global_opt_character_parr.h"

#include <mpi.h>

#include <cmath>
#include <functional>
#include <iostream>
#include <list>

const int nomOfFirstDots = 4;
const double parameterR = 50.0;
const int parameterK = 4;

struct Dot {
    double x;
    double foo;

    explicit Dot(double _foo = 0.0, double _x = 0.0) {
        this->x = _x;
        this->foo = _foo;
    }
};

struct Segment {
    Dot a;
    Dot b;
    double character;

    explicit Segment(Dot _a, Dot _b, double _character) {
        this->a = _a;
        this->b = _b;
        this->character = _character;
    }
};

struct oneDecOptimization {
    std::list<Segment> GlobalSegments;
    std::list<Dot> GlobalDots;
    double exitParamter;
    double M;
    double GlobalA;
    double GlobalB;
    double GlobalY;
    std::function<double(double, double)> GlobalFoo;
    oneDecOptimization(double _exitParamter, double a, double b,
                       std::function<double(double, double)> foo, double y) {
        this->GlobalA = a;
        this->GlobalB = b;
        this->GlobalY = y;
        this->GlobalFoo = foo;
        this->exitParamter = _exitParamter;
        std::list<Dot> dots;
        double eps = std::abs(b - a) / nomOfFirstDots;
        double currX = a;
        double _M = std::abs((foo(currX, y) - foo(currX + eps, y)) / eps);
        dots.push_back(Dot(foo(currX, y), currX));
        currX += eps;
        while (currX < b + this->exitParamter) {
            dots.push_back(Dot(foo(currX, y), currX));
            double localM =
                std::abs((foo(currX, y) - foo(currX + eps, y)) / eps);
            if (localM > _M) _M = localM;
            currX += eps;
        }
        if (_M == 0)
            _M = 1;
        else
            _M *= parameterR;
        this->M = _M;
        for (std::list<Dot>::iterator i = dots.begin(); i != dots.end(); ++i) {
            if (i == dots.begin()) continue;

            std::list<Dot>::iterator j = i;
            --j;

            double character = eps +
                               (pow(i->foo - j->foo, 2) / (eps * pow(_M, 2))) -
                               (2 * (i->foo + j->foo) / _M);
            this->GlobalSegments.push_back(Segment(*j, *i, character));
        }
        this->GlobalDots = dots;
    }
    void sortByCharecter() {
        this->GlobalSegments.sort([](const Segment &a, const Segment &b) {
            return a.character > b.character;
        });
    }

    double IsEnd() {
        for (std::list<Dot>::iterator i = this->GlobalDots.begin();
             i != this->GlobalDots.end(); ++i) {
            if (i == this->GlobalDots.begin()) continue;
            std::list<Dot>::iterator j = i;
            --j;
            if (std::abs(i->x - j->x) < this->exitParamter) {
                return (i->x + j->x) / 2;
            }
        }
        return this->GlobalA - 1;
    }

    void nextIter() {
        this->sortByCharecter();
        std::list<Segment>::iterator iter = this->GlobalSegments.begin();
        for (int i = 0; i < parameterK; i++) {
            if ((iter->a.x == GlobalA) || (iter->b.x == GlobalB)) {
                double x = (iter->a.x + iter->b.x) / 2;
                this->GlobalDots.push_back(
                    Dot(this->GlobalFoo(x, this->GlobalY), x));
                ++iter;
                continue;
            }
            double x = ((iter->a.x + iter->b.x) / 2) -
                       ((iter->b.foo - iter->a.foo) / (2 * this->M));
            this->GlobalDots.push_back(
                Dot(this->GlobalFoo(x, this->GlobalY), x));
            ++iter;
        }
        this->sortDots();
        this->genNewSegments();
    }

    double optimizationStart() {
        while (this->IsEnd() < this->GlobalA) {
            this->nextIter();
        }
        return this->IsEnd();
    }

    void genNewSegments() {
        double M = 0;
        for (std::list<Dot>::iterator i = this->GlobalDots.begin();
             i != this->GlobalDots.end(); ++i) {
            if (i == this->GlobalDots.begin()) continue;
            std::list<Dot>::iterator j = i;
            --j;
            double eps = std::abs(i->x - j->x);
            double lipshic = std::abs(i->foo - j->foo) / eps;
            M = std::max(M, lipshic);
        }
        if (M == 0)
            M = 1;
        else
            M *= parameterR;
        std::list<Segment> segments;
        this->M = M;
        for (std::list<Dot>::iterator i = this->GlobalDots.begin();
             i != this->GlobalDots.end(); ++i) {
            if (i == this->GlobalDots.begin()) continue;
            std::list<Dot>::iterator j = i;
            --j;
            double eps = std::abs(i->x - j->x);
            double character = eps +
                               (pow(i->foo - j->foo, 2) / (eps * pow(M, 2))) -
                               (2 * (i->foo + j->foo) / M);
            segments.push_back(Segment(*j, *i, character));
        }
        this->GlobalSegments = segments;
    }

    void sortDots() {
        this->GlobalDots.sort(
            [](const Dot &a, const Dot &b) { return a.x > b.x; });
    }
};

void optimization(double flag, double exitParamter, double a, double b,
                  std::function<double(double, double)> foo) {
    while (true) {
        double y;
        double *res = new double[2];
        MPI_Status status;
        MPI_Recv(&y, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
        if (y < flag) return;
        oneDecOptimization opt(exitParamter, a, b, foo, y);
        auto s = opt.optimizationStart();
        res[0] = foo(s, y);
        res[1] = y;
        MPI_Send(res, 2, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
        delete[] res;
    }
}

std::list<Dot> genFirstDots(double exitParamter, double ay, double by,
                            double ax, double bx,
                            std::function<double(double, double)> foo) {
    std::list<Dot> res;
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    double eps = (by - ay) / nomOfFirstDots;
    double currY = ay;
    int i = 1;
    if (size > 1) {
        while (currY < by + exitParamter) {
            MPI_Ssend(&currY, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
            i++;
            if (i == size) i = 1;
            currY += eps;
        }
        for (int i = 0; i < nomOfFirstDots + 1; i++) {
            double *buf = new double[2];
            MPI_Status status;
            MPI_Recv(buf, 2, MPI_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD,
                     &status);
            res.push_back(Dot(buf[0], buf[1]));
        }
    } else {
        while (currY < by + exitParamter) {
            oneDecOptimization opt(exitParamter, ax, bx, foo, currY);
            double optX = opt.optimizationStart();
            res.push_back(Dot(foo(optX, currY), currY));
            currY += eps;
        }
    }
    res.sort([](const Dot &a, const Dot &b) { return a.x > b.x; });
    return res;
}

std::list<Segment> segsFromDots(std::list<Dot> dots, double M) {
    std::list<Segment> res;
    for (std::list<Dot>::iterator i = dots.begin(); i != dots.end(); ++i) {
        if (i == dots.begin()) continue;
        std::list<Dot>::iterator j = i;
        --j;
        double eps = std::abs(i->x - j->x);
        double character = eps + (pow(i->foo - j->foo, 2) / (eps * pow(M, 2))) -
                           (2 * (i->foo + j->foo) / M);
        res.push_back(Segment(*j, *i, character));
    }
    return res;
}

double isEnd(double a, std::list<Dot> dots, double exitParamter) {
    for (std::list<Dot>::iterator i = dots.begin(); i != dots.end(); ++i) {
        if (i == dots.begin()) continue;
        std::list<Dot>::iterator j = i;
        --j;
        if (std::abs(i->x - j->x) < exitParamter) {
            return (i->x + j->x) / 2;
        }
    }
    return a - 1;
}

double getMparameter(std::list<Dot> dots) {
    double M = 0;
    for (std::list<Dot>::iterator i = dots.begin(); i != dots.end(); ++i) {
        if (i == dots.begin()) continue;
        std::list<Dot>::iterator j = i;
        --j;
        double lipshic = std::abs((i->foo - j->foo) / (i->x - j->x));
        M = std::max(M, lipshic);
    }
    if (M == 0)
        M = 1;
    else
        M *= parameterR;
    return M;
}

double *twoDecOptimization(double flag, double exitParamter, double ay,
                           double by, double ax, double bx,
                           std::function<double(double, double)> foo) {
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    std::list<Dot> dots = genFirstDots(exitParamter, ay, by, ax, bx, foo);
    double M = getMparameter(dots);
    std::list<Segment> segments = segsFromDots(dots, M);
    while (isEnd(ay, dots, exitParamter) < ay) {
        segments.sort([](const Segment &a, const Segment &b) {
            return a.character > b.character;
        });
        if (size > 1) {
            std::list<Segment>::iterator iter = segments.begin();
            int j = 1;
            for (int i = 0; i < parameterK; i++) {
                double dot;
                if ((iter->a.x == ay) || (iter->b.x == by))
                    dot = (iter->a.x + iter->b.x) / 2;
                else
                    dot = ((iter->a.x + iter->b.x) / 2) -
                          ((iter->b.foo - iter->a.foo) / (2 * M));
                MPI_Ssend(&dot, 1, MPI_DOUBLE, j, 0, MPI_COMM_WORLD);
                j++;
                if (j == size) j = 1;
                ++iter;
            }
            for (int i = 0; i < parameterK; i++) {
                double *buf = new double[2];
                MPI_Status status;
                MPI_Recv(buf, 2, MPI_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD,
                         &status);
                dots.push_back(Dot(buf[0], buf[1]));
                delete[] buf;
            }
        } else {
            std::list<Segment>::iterator iter = segments.begin();
            for (int i = 0; i < parameterK; i++) {
                double dot;
                if ((iter->a.x == ay) || (iter->b.x == by))
                    dot = (iter->a.x + iter->b.x) / 2;
                else
                    dot = ((iter->a.x + iter->b.x) / 2) -
                          ((iter->b.foo - iter->a.foo) / (2 * M));
                oneDecOptimization opt(exitParamter, ax, bx, foo, dot);
                double optX = opt.optimizationStart();

                dots.push_back(Dot(foo(optX, dot), dot));
                ++iter;
            }
        }
        dots.sort([](const Dot &a, const Dot &b) { return a.x > b.x; });
        M = getMparameter(dots);
        segments = segsFromDots(dots, M);
    }
    for (int i = 1; i < size; i++) {
        double endWork = flag - 1;
        MPI_Ssend(&endWork, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
    }
    double resY = isEnd(ay, dots, exitParamter);
    oneDecOptimization opt(exitParamter, ax, bx, foo, 0);
    double resX = opt.optimizationStart();

    double *result = new double[2];
    result[0] = resX;
    result[1] = resY;
    return result;
}
