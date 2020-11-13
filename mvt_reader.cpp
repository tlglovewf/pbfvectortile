#include <mvt_utils.hpp>

class mvtpbf_reader
{
public:
   mvtpbf_reader(const std::string &path):mpath(path)
   {
   }

   void getVectileData(std::vector<vtzero::GeoItemPtr> &geoms)
   {
      const auto data = mvt_pbf::read_file(mpath);
      vtzero::vector_tile tile(data);
      while (auto layer = tile.next_layer()) 
      {
          while (auto feature = layer.next_feature()) 
          {
              vtzero::GeoItemPtr item(mvt_pbf::make_MvtGeomItem(feature.geometry_type()));
              vtzero::decode_geometry(feature.geometry(),item);
              geoms.push_back(item);
          }
      }
      std::cout << "_______________________________ " << geoms.size() << "___________________________" << std::endl;
   }
protected:
   std::string mpath;
};

int main(int argc, char* argv[]) {
      mvtpbf_reader reader(argv[1]);
      std::vector<vtzero::GeoItemPtr> geoms;
      reader.getVectileData(geoms);
    return 0;
}

