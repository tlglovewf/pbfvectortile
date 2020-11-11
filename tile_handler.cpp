#include <iostream>
#include <utils.hpp>
#include <vector_tile.hpp>
using namespace std;

int main(int argc, char **argv)
{
    if(argc < 2)
 	{
       cout << "params error~" << endl;
       return -1;
    }
    else
    {
       const auto data = mvt_pbf::read_file(argv[1]);
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

               vtzero::GeoItemPtr item(mvt_pbf::make_GeomItem(feature.geometry_type()));
               vtzero::decode_geometry(feature.geometry(),item);
               return true;
            });
         }
         
       }
       

    }
   return 0;
}
