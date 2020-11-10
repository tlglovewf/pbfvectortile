// #ifndef GEOMITEM_H
// #define GEOMITEM_H

// #include <vtzero/vector_tile.hpp>

// class geom_point : public geom_item
// {
// public:

//     virtual vtzero::GeomType type()const override
//     {
//         return vtzero::GeomType::POINT;
//     }
    
//     virtual void begin(uint32_t count) override
//     {

//     }

//     virtual void point(const vtzero::point &pt) override
//     {

//     }

//     virtual void end(vtzero::ring_type type = vtzero::ring_type::invalid) override
//     {

//     }
// };

// class geom_line : public geom_item
// {
// public:

//     virtual vtzero::GeomType type()const override
//     {
//         return vtzero::GeomType::LINESTRING;
//     }
    
//     virtual void begin(uint32_t count) override
//     {

//     }

//     virtual void point(const vtzero::point &pt) override
//     {

//     }

//     virtual void end(vtzero::ring_type type = vtzero::ring_type::invalid) override
//     {

//     }
// };

// class geom_polygon : public geom_item
// {
// public:

//     virtual vtzero::GeomType type()const override
//     {
//         return vtzero::GeomType::POLYGON;
//     }
    
//     virtual void begin(uint32_t count) override
//     {

//     }

//     virtual void point(const vtzero::point &pt) override
//     {

//     }

//     virtual void end(vtzero::ring_type type = vtzero::ring_type::invalid) override
//     {

//     }
// };

//     static std::shared_ptr<geom_item> make_GeomItem(vtzero::GeomType type)
//     {
//         switch (type)
//         {
//         case vtzero::GeomType::POINT:
//             return std::shared_ptr<geom_item>(new geom_point());
//         case vtzero::GeomType::LINESTRING:
//             return std::shared_ptr<geom_item>(new geom_line());
//         case vtzero::GeomType::POLYGON:
//             return std::shared_ptr<geom_item>(new geom_polygon());
//         default:
//             assert(NULL);
//             break;
//         }
//     }
// #endif