#include <iostream>
#include <fstream>
#include <vector>

#ifndef BZ
#pragma GCC optimize ("Ofast")
#pragma GCC optimize ("unroll-loops")
#pragma GCC optimize ("no-stack-protector")
#pragma GCC target ("tune=native")
#endif

#define RELEASE

#define end "\n"
#ifndef RELEASE
#define DEBUG
#define DEBUG_OUTPUT(what) std::cout <<  "DEBUG: " <<  what
#endif

const char* SKIP_FLAG = "-t";

typedef unsigned int UInt; // для красоты кода

namespace Sort {

    template<typename T>
    class Comparator {
    public:
        static inline bool compare(const T first, const T second) {
            return first >= second;
        }
    };

    template<typename T, class comparator = Comparator<T>>
    void gnome_sort(std::vector<T> &array) {
#ifdef DEBUG
        DEBUG_OUTPUT("gnome_sort") <<  end;
#endif
        size_t i = 0;
        while (i < array.size()) {
            auto comp =  comparator::compare(array[i], array[i - 1]);
            if (i == 0 || comp) {
                i++;
                continue;
            }
            std::swap(array[i], array[i - 1]);
            i--;
        }
#ifdef DEBUG
        DEBUG_OUTPUT("END") <<  end;
#endif
    }
}

namespace get_data {
    static const std::string DELIMITER = " ";

    inline auto read_file(const std::string &path = "test.txt") -> std::string {
#ifdef DEBUG
        DEBUG_OUTPUT("read_file") <<  end;
#endif
        std::ifstream stream(path);
        stream.exceptions(std::ios_base::badbit);
#ifdef DEBUG
        if (!stream) {
            std::cout <<  "File not create" <<  end;
        }
#endif
        std::string buffer;
        std::string line;
        while (std::getline(stream, line)) {
#ifdef DEBUG
            std::cout <<  line <<  " ";
#endif
            std::cout <<  end;
            buffer.append(line);
        }
#ifdef DEBUG
        std::cout <<  "buffer is: " <<  buffer <<  end;
#endif
        return buffer;
    }

    template<typename T>
    inline auto get_number_vector_from_string(const std::string &buffer) -> std::vector<T> {
#ifdef DEBUG
        DEBUG_OUTPUT("get_number_vector_from_string") <<  end;
#endif
        std::vector<T> out_buffer;
        auto start = 0U;
        auto ends = buffer.find(DELIMITER);
        while (ends != std::string::npos) {
            try {
                out_buffer.template emplace_back(std::stoi(buffer.substr(start, ends - start)));

            } catch (const std::exception&) {
//обратка ошибок
            }
            start = ends + DELIMITER.length();
            ends = buffer.find(DELIMITER, start);
        }
        try {
            out_buffer.template emplace_back(std::stoi(buffer.substr(start, ends)));
        } catch (const std::exception&) {
//обратка ошибок
        }
#ifdef DEBUG
        DEBUG_OUTPUT("ARRAY") <<  end;
#pragma omp parallel for num_threads(8)
        for (auto r: out_buffer) {
            std::cout <<  r <<  " ";
        }
        std::cout <<  end;
        DEBUG_OUTPUT("END") <<  end;
#endif
        return out_buffer;
    }
}

int main(int argc, char** argv) {
#ifdef RELESE
    const char *name_file = argv[0];
const char *output_file_name = argv[1];
#else
    const char *name_file = "../test.txt";
    const char *output_file_name = "../out.txt"; // указывай путь для дебаг файла тут (вывод)
#endif
    UInt start_time = clock();
    std::ofstream out_file;
    out_file.open(output_file_name); // открывается файл для записи овтета
    if (argc > 1) {
        if (char *flag = argv[2]; flag == SKIP_FLAG) {
            out_file.open(output_file_name);
            out_file << "0 0 0"; // запись в файл
            out_file.close();
        }
    }
    std::vector<int> on_sort;
    std::string buffer = get_data::read_file(name_file);
    std::vector<int> res = get_data::get_number_vector_from_string<int>(buffer);
    Sort::gnome_sort<int>(res);
    UInt end_time = clock();
    UInt search_time = end_time - start_time;
    if (res.empty()) {
        out_file << " ";
        return 0;
    }

    std::cout <<  search_time << " " <<  end;

// запись результата в файл
    for (auto i: res)
        out_file <<  i <<  "\t";
    out_file.close(); // untitled/cmake-build-debug/out.txt тут записан ответ
#ifdef DEBUG
    DEBUG_OUTPUT("sort result: ") <<  " ";
#pragma omp parallel for num_threads(8)
    for (auto i: res) {
        std::cout <<  i <<  " ";
    }
#endif
    return 0;
}
