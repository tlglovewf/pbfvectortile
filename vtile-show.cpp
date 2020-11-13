/*****************************************************************************

  Example program for vtzero library.

  vtzero-show - Show content of vector tile

*****************************************************************************/

#include <mvt_utils.hpp>

#include <vector_tile.hpp>

#include <clara.hpp>

#include <cstdint>
#include <exception>
#include <iostream>
#include <string>

/**
 * Get a specific layer from a vector tile. The layer can be specified as a
 * number n in which case the nth layer in this tile is returned. Or it can
 * be specified as text, in which case the layer with that name is returned.
 *
 * Calls exit(1) if there is an error.
 *
 * @param tile The vector tile.
 * @param layer_name_or_num specifies the layer.
 */
vtzero::layer get_layer(const vtzero::vector_tile& tile, const std::string& layer_name_or_num) {
    vtzero::layer layer;
    char* str_end = nullptr;
    const long num = std::strtol(layer_name_or_num.c_str(), &str_end, 10); // NOLINT(google-runtime-int)

    if (str_end == layer_name_or_num.data() + layer_name_or_num.size()) {
        if (num >= 0 && num < std::numeric_limits<long>::max()) { // NOLINT(google-runtime-int)
            layer = tile.get_layer(static_cast<std::size_t>(num));
            if (!layer) {
                std::cerr << "No such layer: " << num << '\n';
                std::exit(1);
            }
            return layer;
        }
    }

    layer = tile.get_layer_by_name(layer_name_or_num);
    if (!layer) {
        std::cerr << "No layer named '" << layer_name_or_num << "'.\n";
        std::exit(1);
    }
    return layer;
}

class geom_point : public vtzero::geom_point
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


template <typename TChar, typename TTraits>
std::basic_ostream<TChar, TTraits>& operator<<(std::basic_ostream<TChar, TTraits>& out, vtzero::data_view value) {
    out.write(value.data(), static_cast<std::streamsize>(value.size()));
    return out;
}

struct print_value {

    template <typename T>
    void operator()(T value) const {
        std::cout << value;
    }

    void operator()(const vtzero::data_view value) const {
        std::cout << '"' << value << '"';
    }

}; // struct print_value

static void print_layer(vtzero::layer& layer, bool print_tables, bool print_value_types, int layer_num, int& feature_num) {
    std::cout << "=============================================================\n"
              << "layer: " << layer_num << '\n'
              << "  name: " << std::string(layer.name()) << '\n'
              << "  version: " << layer.version() << '\n'
              << "  extent: " << layer.extent() << '\n';

    if (print_tables) {
        std::cout << "  keys:\n";
        int n = 0;
        for (const auto& key : layer.key_table()) {
            std::cout << "    " << n++ << ": " << key << '\n';
        }
        std::cout << "  values:\n";
        n = 0;
        for (const vtzero::property_value& value : layer.value_table()) {
            std::cout << "    " << n++ << ": ";
            vtzero::apply_visitor(print_value{}, value);
            if (print_value_types) {
                std::cout << " [" << vtzero::property_value_type_name(value.type()) << "]\n";
            } else {
                std::cout << '\n';
            }
        }
    }

    feature_num = 0;
    while (auto feature = layer.next_feature()) {
        std::cout << "  feature: " << feature_num << '\n'
                  << "    id: ";
        if (feature.has_id()) {
            std::cout << feature.id() << '\n';
        } else {
            std::cout << "(none)\n";
        }
        std::cout << "    geomtype: " << vtzero::geom_type_name(feature.geometry_type()) << '\n'
                  << "    geometry:\n";
        vtzero::GeoItemPtr item(make_GeomItem(feature.geometry_type()));
        vtzero::decode_geometry(feature.geometry(),item);
        std::cout << "    properties:\n";
        while (auto property = feature.next_property()) {
            std::cout << "      " << property.key() << '=';
            vtzero::apply_visitor(print_value{}, property.value());
            if (print_value_types) {
                std::cout << " [" << vtzero::property_value_type_name(property.value().type()) << "]\n";
            } else {
                std::cout << '\n';
            }
        }
        ++feature_num;
    }
}

static void print_layer_overview(const vtzero::layer& layer) {
    std::cout << layer.name() << ' ' << layer.num_features() << '\n';
}

int main(int argc, char* argv[]) {
    std::string filename;
    std::string layer_num_or_name;
    bool layer_overview = false;
    bool print_tables = false;
    bool print_value_types = false;
    bool help = false;

    const auto cli
        = clara::Opt(layer_overview)
            ["-l"]["--layers"]
            ("show layer overview with feature count")
        | clara::Opt(print_tables)
            ["-t"]["--tables"]
            ("also print key/value tables")
        | clara::Opt(print_value_types)
            ["-T"]["--value-types"]
            ("also show value types")
        | clara::Help(help)
        | clara::Arg(filename, "FILENAME").required()
            ("vector tile")
        | clara::Arg(layer_num_or_name, "LAYER-NUM|LAYER-NAME")
            ("layer");

    const auto result = cli.parse(clara::Args(argc, argv));
    if (!result) {
        std::cerr << "Error in command line: " << result.errorMessage() << '\n';
        return 1;
    }

    if (help) {
        std::cout << cli
                  << "\nShow contents of vector tile FILENAME.\n";
        return 0;
    }

    if (filename.empty()) {
        std::cerr << "Error in command line: Missing file name of vector tile to read\n";
        return 1;
    }

    int layer_num = 0;
    int feature_num = 0;
    try {
        const auto data = mvt_pbf::read_file(filename);

        vtzero::vector_tile tile{data};

        if (layer_num_or_name.empty()) {
            while (auto layer = tile.next_layer()) {
                if (layer_overview) {
                    print_layer_overview(layer);
                } else {
                    print_layer(layer, print_tables, print_value_types, layer_num, feature_num);
                }
                ++layer_num;
            }
        } else {
            auto layer = get_layer(tile, layer_num_or_name);
            if (layer_overview) {
                print_layer_overview(layer);
            } else {
                print_layer(layer, print_tables, print_value_types, layer_num, feature_num);
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error in layer " << layer_num << " (feature " << feature_num << "): " << e.what() << '\n';
        return 1;
    }

    return 0;
}

