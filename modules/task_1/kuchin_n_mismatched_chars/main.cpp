// Copyright 2022 Kuchin Nikita

#include <gtest/gtest.h>

#include <gtest-mpi-listener.hpp>

#include "./mismatched_chars.h"

TEST(MPI_1, 26str) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::string f = "3FDkfkPDG7M0WMvX3fUY2thEh";
    std::string s = "3FD1nHopZ2NFp7TpuVvQAxn4P";
    int resp = result(f, s, f.size());

    if (rank == 0) {
        int ress = straightsum(f, s, f.size(), 0, f.size());
        ASSERT_EQ(resp, ress);
    }
}
TEST(MPI_1, 15str) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::string f = "3RRRjXJ9jZi9Giv";
    std::string s = "waJVgHFZt9bCZ4z";
    int resp = result(f, s, 16);

    if (rank == 0) {
        int ress = straightsum(f, s, 16, 0, 16);
        ASSERT_EQ(resp, ress);
    }
}

TEST(MPI_1, str100) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int size = 101;

    std::string first, second;
    first =
        "3ikD73kHI8iJaIVverIYl0yaxbf3pwk8YbWD7lLcpwAn8JLVGInABBa8Vbk2nc3ziNpguW"
        "2bHUF21LbF6TvbUNHnkl7DCpNqDIXD";
    second =
        "CYN9gOIc8uKKUamE39Ngb4i5SvAD7G59jiSCeI3k0EJ8A6DkSk3SBNbZCsEt3qhG0nB8bW"
        "spapbyLsBN1PCAjGlbCNxX7eYDBtuS";
    int resp = result(first, second, size);

    if (rank == 0) {
        int ress = straightsum(first, second, size, 0, size);
        ASSERT_EQ(resp, ress);
    }
}

TEST(MPI_1, str500) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int size = 501;

    std::string first, second;
    first =
        "DnYKRiKSuN9HzmBW0eFIMxTnC1sy290cIBSnY63JTrC7K9KCjbv1wHaMxe0oiEtleL62ot"
        "bLo73dZeGpMqpuiLt1cDKFQlB06MHXuwHFISASn7f1Zg9v9413IPgQHTEBGV2T0lQ6GPYJ"
        "dxMuUUee5uAKFUcE5TpBRBkceHXNzxSCrwthhseiSq1pzpxBlqQur95ZwxKgbK3pMG2QbO"
        "zcZ7dx2srFucefOhEMEATvMBrQieHWW1Jc4ye60i9nHbOVArHWdigAWllJai4fRwrUmcC5"
        "UHelkdms0R6LYreC0tTHwJk4ChLtiNq1GBvDBF1WTZx0Yfs0cDm6svnWkEtNzYzzkdBset"
        "SKgthtyQXUfNQzvkORadC4GeOUmjnNaurtQauvZf0hODj4iK7REPhTN1fmIEZifSSRDjWc"
        "LZEn5a8zjMME5IFkhA6pK9dsv1FyiV3wpRWYBBQNNb8OPgF0UZKHjo5Qzl5AdkkssHjc5u"
        "VhKtz2LvlP";
    second =
        "xmCtp8YuAq6owjAcd4sfqeyupLWY6fXKokAx8acZwtWTbOdYXjj2vwEo1qDKjSYCOmUOn7"
        "YRMQSw9Xm5EbwNHAUTp4qIgt1wiOr6zoIPO8CBeuZczLsyNEiXofKpJfU0GykqhQkw77N7"
        "jC7louQkmjPoDWFJkAp7HMxOSDcRq3SGYk50gUq3etrsficMIzqWyfrnBNDin1zVH8hhJu"
        "DJlPG68yoFKKQZc595m3Q3NXTr8z6ttlPFGqIqf1w2LoJHDhS9bCACPzet4LdAu5ywnN15"
        "bfZkY1LVm6w4raAhyWNe0MnksW0MOocTKuYfijg6WsmxsivLdZOWxlCUHxqHnbEFNuy1sw"
        "9wa37H0Tt1Gf4Me0qo8ft2KIQENPo0IyGZrHTdYT7IVVIFClWZWReiDi3RsemIElVK3dDC"
        "vgBqhVmEYKZtLFMp3P5ijhuRQG3yRkhE1CimDRT7Q3DeLHOWF09FCOD2qtquhi8DVq0rnc"
        "bQ9rBsVREd";
    int resp = result(first, second, size);

    if (rank == 0) {
        int ress = straightsum(first, second, size, 0, size);
        ASSERT_EQ(resp, ress);
    }
}

TEST(MPI_1, str1000) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int size = 1001;

    std::string first, second;
    first =
        "o1RDu0jjY1tUeoWxXhxFjy4qBNhhNiWN0CpnvvNLKaG50LSwd0wdF7wjKsL3kYCbyMkrH"
        "3h1HNqCSUqA21Q50jy5byHQ0FHrPEXa9htZPnpWhSvMGIQrhAOog7W8s5ms3Pys5EIv9JZ"
        "0FYyuJ0QYzG0U4MSyvWwGOziAkNqtQHlT844yTg7QHq5fazLgaajjvMuu3In3KlwCciGs3"
        "Oxrg2Tsomjq1w6vXJ0hGwQjSzK6pGwWwW5e9aKL6QJdz7TpqB7KeMrgCs0UZep9xQGIz7h"
        "5o3wv2GiQhOq2RKeNzVtwGPvJS9EWytJOOiPsrWdAJ1KwOUagJbMmE00AKAgPzMqI2sm8y"
        "2ahJaDzd8Tck4azuDPAONQGu2WWauPHz7FIpgHv9AlUv7rnOWkFrjfgraEG1QXKNsQh8E0"
        "xuQzYDpl6nlH7Wpgjx5GnjSmOKVzTE2dBXBvC7gjou3IJr5UZka1S1ka4O3xLEelYdWvy2"
        "yZXpMwfUadBs74pODW5g1uQcCC8LOSkk7gjhoikriZ4c0970CmPLaoG76gTeb3lvvQEMew"
        "F9vUFGc1WUIcbKS8v5PvLR52cBeuHXx7CBBJSAgyvT9K5IebwY5cwE2y28jUD9WRUzOH3l"
        "ez3qcgEXpRhU1G7zg7ZW0Rvb33UIWqD29dUWJNvVshqlMRuoCDmlHPRcObtxP1MAxBdeP2"
        "qTCgLzNHlrutucQRGDRKMEOLJgCC5yoZ17osxXOuvwOvybfbH5ovzhN1AQvvZLcoHtJ4bp"
        "BHgcfH7HsejWp2mLihTGtfWYGco5UD3QdShWCniKvsrnNTgED1RhlQVHLK6uTymCyvmX5N"
        "aUtPxrqoI2qyy9ZsdGplm98MA8Zl96ZdarQXDr05Vzq11kvXAYOMe8VwXKZwyCnchRsrBi"
        "GBDOqxwJPr5D2ttSAwEfWDa0YtnGGTEBKQF7Jmfeby6xSngsNgPEtT7x1qE47K3UO6PUHz"
        "7jAGLNBq79DFlP14pBFga";
    second =
        "HFBXUzmf8s7Cbd9fFdOaIcJVXKbJ14t5x4WvpZsMZCcqXZEqNJkpbO2DNrguJHXIrmZL1I"
        "Zqqk2hoqg8mpfwDWddoCQ0nSIWfUycpaQ4NxPJxNUzC3p4EQ1QS15zVSX1i8fwmrI8Zw5p"
        "TFpokzsShVLtsrIBb7L0UreLj5KvudpEZU4BdDCOonITTbMPtCFhoh89vHOZxmUUxslZAJ"
        "Xh8mbi6ry7aeofKJLeBv9WXvuSSySpNlRVkWZBfeB9aCqRtBDUbGqLkKMqicegiZkBelbb"
        "B8hA1iURhBM3vHajT33u2Wy4D56UDLG4e3Ezeg3Bb7woWdauX2NAjefOZz0Kzi3hjmfA53"
        "IYo1HnJ5pipYa1M152W2mB63AMRV1ld8bkJmQOhxN4lnGzCZcJ2LEOz3VCr2LppnMKk4hA"
        "DZ8cxX3gHgChLm6jzj9jNF7mglEbvz4Rm0FOTAXnclvwmKZDt9t4wpIqPMbCDzou4ynr2t"
        "3aGGSGwK6lFPzeQDnQIAycVgObZ66xDCcHqUaG9DucEitpa5IyKyGxZg0bXDBi5ivj1mcX"
        "VKC0WTITlnqJiRl8FACJnkRA8CFQKlGHARQcRoVR5QFYb5ZuJH2jg7hS4v06cwaABodK0T"
        "h3iPJhHAwEAbuQJFVtAohjwhhKi2nexekh8SMIAZIviwtnZcfw5Ps1mSbIwzRFYtUHu1Mq"
        "oSGGm6JVmJfHwNZI4IwchbbjtH359uHzJSliHXGC4tYbarRGnY3UFeIIfZp0BWMVbFc5gA"
        "1gkeYvmtoBCvBk9bPU431gGK0RU4p211c6OsmChs8xSHLDZhUOC2URxQPusmu9jhBwyIkj"
        "sYfFje19VYDtcnd5Duf1cvkc6pf30VbQqYO7Ako2NyxE0Y23MWkDwBe9ulk3SmlJbc7bHj"
        "3bv5e7gP1bWT7rQCF3RJuEuXr71rFoGCJqFu5LxlvxLjQjlVxseL7NcS0KXAm2AVDnlfDB"
        "hXxIv4UvlkjgLuq30m6a";
    int resp = result(first, second, size);

    if (rank == 0) {
        int ress = straightsum(first, second, size, 0, size);
        ASSERT_EQ(resp, ress);
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
