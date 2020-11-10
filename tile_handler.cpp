#include <iostream>
#include <utils.hpp>
#include <vector_tile.hpp>
#include <geom_item.hpp>
using namespace std;
 
 
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

static vtzero::GeoItemPtr make_GeomItem(vtzero::GeomType type)
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



int main(int argc, char **argv)
{
    if(argc < 2)
 	{
       cout << "params error~" << endl;
       return -1;
    }
    else
    {
       const auto data = read_file(argv[1]);
       vtzero::vector_tile tile{data};
       const size_t layersz = tile.count_layers();
       cout << "layer size : " << layersz << endl;

       for(size_t i = 0; i < layersz; ++i)
       {
         const vtzero::layer layer = std::move(tile.get_layer(i));

         if(layer)
         {
            cout << layer.name().to_string() << " has " << layer.num_features() << " features~" << endl;

            layer.for_each_feature([](const vtzero::feature &feature)->bool{

               cout << "feature type : " << static_cast<int>(feature.geometry_type()) << endl;

               const vtzero::geometry &geo = feature.geometry();

               vtzero::GeoItemPtr item(make_GeomItem(feature.geometry_type()));
               vtzero::decode_geometry(feature.geometry(),item);
               return true;
            });
         }
         
       }
       

    }
   return 0;
}
