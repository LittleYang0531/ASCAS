#include "../../../ascas.h"

 auto CropsRecordsAdd = [](client_conn conn, http_request request, param argv) {
    if (request.method != "POST") quickSendCode(405);
    int uid = UserUtils.checkLogin(request);
    if(uid == 0) quickSendCode(401);
     Crop crop = CropUtils.getCropInfo(stoi(argv[0]),uid);
    if(crop.permission != UserPermission::OWNER 
        && crop.permission != UserPermission::EDITOR) quickSendCode(401);
    auto posts = json_decode(request.postdata); 
    std::string all,sep = ",",c = "\"";
    size_t n = crop.properties.size();
    std::vector<std::string> vjoin;
    for(int i = 0;i < n; ++i) {
        std::string newline;
        if(crop.properties[i].type == RecordPropertyType::NUMBER) {
            newline += (posts[crop.properties[i].name].asString());
        }
        else {
        newline += c;
        newline += ((posts[crop.properties[i].name].asString()));
        newline += c;
        }
        vjoin.push_back(newline);
    }       
    all = join(sep,vjoin);
    std::cout << all << std::endl;
    quick_mysqli_connect();
    mysqli_execute(
        mysql,
        "insert into table_%s values(%s)",
        quote_encode(crop.name).c_str(),
        all.c_str()
    );
    quickSendCode(200);
};
   

