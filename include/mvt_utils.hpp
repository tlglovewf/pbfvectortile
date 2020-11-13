/*****************************************************************************

  Utility functions for vtzero example programs.

*****************************************************************************/
#include <vtzero/vector_tile.hpp>
#include <geometry.hpp>
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

    /*
     *  point geometry storage
     */
    class mvt_point : public vtzero::geom_point
    {
    public:

        virtual void begin(uint32_t count) override
        {
            
        }

        virtual void setpoint(const vtzero::point &pt) override
        {
            mpt = pt ;
        }

        virtual void end(vtzero::ring_type type = vtzero::ring_type::invalid) override
        {

        }
        virtual const void* data()const 
        {
            return &mpt;
        }
    protected:
        vtzero::point mpt;
    };
    /*
     *  line geometry storage
     */
    class mvt_line : public vtzero::geom_line
    {
    public:
        virtual void begin(uint32_t count) override
        {
            mline._pts.reserve(count);
        }

        virtual void setpoint(const vtzero::point &pt) override
        {
            mline._pts.push_back(pt);
        }

        virtual void end(vtzero::ring_type type = vtzero::ring_type::invalid) override
        {

        }
        virtual const void* data()const 
        {
            return &mline;
        }
    protected:
        vtzero::line mline;
    };

    /*
     *  polygon geometry storage
     */
    class mvt_polygon : public vtzero::geom_polygon
    {
    public:
        mvt_polygon(){}
        ~mvt_polygon(){}
        virtual void begin(uint32_t count) override
        {
            vtzero::ring r;
            r._l._pts.reserve(count);
            mrings._rs.push_back(r);
        }

        virtual void setpoint(const vtzero::point &pt) override
        {
            mrings._rs.rbegin()->_l._pts.push_back(pt);
        }

        virtual void end(vtzero::ring_type type = vtzero::ring_type::invalid) override
        {
            mrings._rs.rbegin()->_t = type;
        }
        virtual const void* data()const 
        {
            return &mrings;
        }
    protected:
        vtzero::polygon         mrings;
    };

    /*
     *  point geometry display
     */ 
    class display_point : public vtzero::geom_point
    {
    public:

        virtual void begin(uint32_t count) override
        {

        }

        virtual void setpoint(const vtzero::point &pt) override
        {
            std::cout << "      POINT(" << pt.x << ',' << pt.y << ")\n";
        }

        virtual void end(vtzero::ring_type type = vtzero::ring_type::invalid) override
        {

        }
    };

    /*
     *  line geometry display
     */ 
    class display_line : public vtzero::geom_line
    {
        std::string output{};
    public:
        virtual void begin(uint32_t count) override
        {
            output = "      LINESTRING[count=";
            output += std::to_string(count);
            output += "](";
        }

        virtual void setpoint(const vtzero::point &pt) override
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

    /*
     *  polygon geometry display
     */ 
    class display_polygon : public vtzero::geom_polygon
    {
        std::string output{};
    public:
        virtual void begin(uint32_t count) override
        {
            output = "      RING[count=";
            output += std::to_string(count);
            output += "](";
        }

        virtual void setpoint(const vtzero::point &pt) override
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

    /*
     *   generate a geometry data storage object
     * 
     */ 
    inline vtzero::GeoItemPtr make_MvtGeomItem(vtzero::GeomType type)
    {
        switch (type)
        {
        case vtzero::GeomType::POINT:
            return vtzero::GeoItemPtr(new mvt_point());
        case vtzero::GeomType::LINESTRING:
            return vtzero::GeoItemPtr(new mvt_line());
        case vtzero::GeomType::POLYGON:
            return vtzero::GeoItemPtr(new mvt_polygon());
        default:
            assert(NULL);
            break;
        }
    }

    /*
     *   generate a geometry data display object
     * 
     */ 
    inline vtzero::GeoItemPtr make_DisplayGeomItem(vtzero::GeomType type)
    {
        switch (type)
        {
        case vtzero::GeomType::POINT:
            return vtzero::GeoItemPtr(new display_point());
        case vtzero::GeomType::LINESTRING:
            return vtzero::GeoItemPtr(new display_line());
        case vtzero::GeomType::POLYGON:
            return vtzero::GeoItemPtr(new display_polygon());
        default:
            assert(NULL);
            break;
        }
    }
}