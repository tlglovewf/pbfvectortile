/*****************************************************************************

  Utility functions for vtzero example programs.

*****************************************************************************/
#include <vtzero/vector_tile.hpp>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>

namespace mvt_pbf
{
    /**
     * Read complete contents of a file into a string.
     *
     * The file is read in binary mode.
     *
     * @param filename The file name. Can be empty or "-" to read from STDIN.
     * @returns a string with the contents of the file.
     * @throws various exceptions if there is an error
     */
    std::string read_file(const std::string& filename) {
        if (filename.empty() || (filename.size() == 1 && filename[0] == '-')) {
            return std::string{std::istreambuf_iterator<char>(std::cin.rdbuf()),
                            std::istreambuf_iterator<char>()};
        }

        std::ifstream stream{filename, std::ios_base::in | std::ios_base::binary};
        if (!stream) {
            throw std::runtime_error{std::string{"Can not open file '"} + filename + "'"};
        }

        stream.exceptions(std::ifstream::failbit);

        std::string buffer{std::istreambuf_iterator<char>(stream.rdbuf()),
                        std::istreambuf_iterator<char>()};
        stream.close();

        return buffer;
    }

    /**
     * Write contents of a buffer into a file.
     *
     * The file is written in binary mode.
     *
     * @param buffer The data to be written.
     * @param filename The file name.
     * @throws various exceptions if there is an error
     */
    void write_data_to_file(const std::string& buffer, const std::string& filename) {
        std::ofstream stream{filename, std::ios_base::out | std::ios_base::binary};
        if (!stream) {
            throw std::runtime_error{std::string{"Can not open file '"} + filename + "'"};
        }

        stream.exceptions(std::ifstream::failbit);

        stream.write(buffer.data(), static_cast<std::streamsize>(buffer.size()));

        stream.close();
    }

    /****
     *   define geom reader class 
     * 
     * */


    class geom_point : public vtzero::geom_point
    {
    public:

        virtual void begin(uint32_t count) override
        {

        }

        virtual void point(const vtzero::point &pt) override
        {
            std::cout << "      POINT(" << pt.x << ',' << pt.y << ")\n";
        }

        virtual void end(vtzero::ring_type type = vtzero::ring_type::invalid) override
        {

        }
    };

    class geom_line : public vtzero::geom_line
    {
        std::string output{};
    public:
        virtual void begin(uint32_t count) override
        {
            output = "      LINESTRING[count=";
            output += std::to_string(count);
            output += "](";
        }

        virtual void point(const vtzero::point &pt) override
        {
            output += std::to_string(pt.x);
            output += ' ';
            output += std::to_string(pt.y);
            output += ',';
        }

        virtual void end(vtzero::ring_type type = vtzero::ring_type::invalid) override
        {
            if (output.empty()) {
                return;
            }
            if (output.back() == ',') {
                output.resize(output.size() - 1);
            }
            output += ")\n";
            std::cout << output;
        }
    };

    class geom_polygon : public vtzero::geom_polygon
    {
        std::string output{};
    public:
        virtual void begin(uint32_t count) override
        {
            output = "      RING[count=";
            output += std::to_string(count);
            output += "](";
        }

        virtual void point(const vtzero::point &pt) override
        {
            output += std::to_string(pt.x);
            output += ' ';
            output += std::to_string(pt.y);
            output += ',';
        }

        virtual void end(vtzero::ring_type type = vtzero::ring_type::invalid) override
        {
            if (output.empty()) {
                return;
            }
            if (output.back() == ',') {
                output.back() = ')';
            }
            switch (type) {
                case vtzero::ring_type::outer:
                    output += "[OUTER]\n";
                    break;
                case vtzero::ring_type::inner:
                    output += "[INNER]\n";
                    break;
                default:
                    output += "[INVALID]\n";
            }
            std::cout << output;
        }

    };

    inline vtzero::GeoItemPtr make_GeomItem(vtzero::GeomType type)
    {
        switch (type)
        {
        case vtzero::GeomType::POINT:
            return vtzero::GeoItemPtr(new geom_point());
        case vtzero::GeomType::LINESTRING:
            return vtzero::GeoItemPtr(new geom_line());
        case vtzero::GeomType::POLYGON:
            return vtzero::GeoItemPtr(new geom_polygon());
        default:
            assert(NULL);
            break;
        }
    }
}