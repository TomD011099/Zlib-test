#include "zlib.h"
#include <iostream>
#include <vector>
#include <chrono>

int level;
std::vector<uint8_t> bus;
std::vector<uint8_t> send;

void compress_test(const std::vector<uint8_t> &payload) {
    std::string str(payload.begin(), payload.end());
    std::cout << str.size() << "\t";

    auto start = std::chrono::steady_clock::now();
    std::vector<uint8_t> buffer;

    const size_t BUFSIZE = 128 * 1024;
    uint8_t temp_buffer[BUFSIZE];

    z_stream strm;
    strm.zalloc = nullptr;
    strm.zfree = nullptr;
    strm.next_in = const_cast<Bytef*>(payload.data());
    strm.avail_in = payload.size();
    strm.next_out = temp_buffer;
    strm.avail_out = BUFSIZE;

    deflateInit(&strm, level);

    while (strm.avail_in != 0) {
        int res = deflate(&strm, Z_NO_FLUSH);
        //assert(res == Z_OK); assert not found???
        if (strm.avail_out == 0) {
            buffer.insert(buffer.end(), temp_buffer, temp_buffer + BUFSIZE);
            strm.next_out = temp_buffer;
            strm.avail_out = BUFSIZE;
        }
    }

    int deflate_res = Z_OK;
    while (deflate_res == Z_OK) {
        if (strm.avail_out == 0) {
            buffer.insert(buffer.end(), temp_buffer, temp_buffer + BUFSIZE);
            strm.next_out = temp_buffer;
            strm.avail_out = BUFSIZE;
        }
        deflate_res = deflate(&strm, Z_FINISH);
    }

    //assert(deflate_res == Z_STREAM_END);
    buffer.insert(buffer.end(), temp_buffer, temp_buffer + BUFSIZE - strm.avail_out);
    deflateEnd(&strm);

    send.swap(buffer);
    auto end = std::chrono::steady_clock::now();

    std::string temp(send.begin(), send.end());
    std::cout << temp.size() << "\t" << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << "\n";
}

void decompress_test(const std::vector<uint8_t> &payload) {
    std::vector<uint8_t> buffer;
    std::vector<uint8_t> out;

    const size_t BUFSIZE = 128 * 1024;
    uint8_t temp_buffer[BUFSIZE];

    z_stream strm;
    strm.zalloc = nullptr;
    strm.zfree = nullptr;
    strm.next_in = const_cast<Bytef*>(payload.data());
    strm.avail_in = payload.size();
    strm.next_out = temp_buffer;
    strm.avail_out = BUFSIZE;

    inflateInit(&strm);

    while (strm.avail_in != 0) {
        int res = inflate(&strm, Z_NO_FLUSH);
        //assert(res == Z_OK); assert not found???
        if (strm.avail_out == 0) {
            buffer.insert(buffer.end(), temp_buffer, temp_buffer + BUFSIZE);
            strm.next_out = temp_buffer;
            strm.avail_out = BUFSIZE;
        }
    }

    int inflate_res = Z_OK;
    while (inflate_res == Z_OK) {
        if (strm.avail_out == 0) {
            buffer.insert(buffer.end(), temp_buffer, temp_buffer + BUFSIZE);
            strm.next_out = temp_buffer;
            strm.avail_out = BUFSIZE;
        }
        inflate_res = deflate(&strm, Z_FINISH);
    }

    //assert(deflate_res == Z_STREAM_END);
    buffer.insert(buffer.end(), temp_buffer, temp_buffer + BUFSIZE - strm.avail_out);
    inflateEnd(&strm);

    out.swap(buffer);
    //this->receive(out);

    std::string str(out.begin(), out.end());
    std::cout << "Rec\n" << str << "\n" << str.size() << "\n";
}

void flush() {
    std::string str(send.begin(), send.end());
    std::cout << "Compr\n" << str << "\n" << str.size() << "\n\n";
    bus = send;
}

void sendData(const std::vector<uint8_t> &payload) {
    compress_test(payload);

    flush();
}

void receiveData(const std::vector<uint8_t> &payload) {
    decompress_test(payload);
}

int main(int argc, char* argv[]) {
    level = 0;

    std::string str = "Hello world!";
    std::vector<uint8_t> payload(str.begin(), str.end());
    compress_test(payload);

    str = "Lorem ipsum dolor sit amet.";
    payload.assign(str.begin(), str.end());
    compress_test(payload);

    str = "Lorem ipsum dolor sit amet, consectetur cras amet.";
    payload.assign(str.begin(), str.end());
    compress_test(payload);

    str = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nulla aliquam sed.";
    payload.assign(str.begin(), str.end());
    compress_test(payload);

    str = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Etiam nec libero massa. Pellentesque metus.";
    payload.assign(str.begin(), str.end());
    compress_test(payload);

    str = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Quisque dignissim maximus turpis at rhoncus. Nunc aliquet nibh sed metus volutpat massa nunc.";
    payload.assign(str.begin(), str.end());
    compress_test(payload);

    str = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nam molestie lobortis magna ac ultrices. Suspendisse in suscipit erat. Nam enim velit, interdum a aliquam eu, interdum in urna. Curabitur amet.";
    payload.assign(str.begin(), str.end());
    compress_test(payload);

    str = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. In risus nulla, malesuada quis lectus congue, pretium cursus nisi. Vestibulum pharetra eros eu vulputate tempus. Morbi id sem eu ligula fermentum dictum vitae ut ipsum. Integer commodo volutpat dictum. In vel erat id massa finibus turpis duis.";
    payload.assign(str.begin(), str.end());
    compress_test(payload);

    str = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Fusce tempus ac eros in tempor. Sed quis enim sem. Nam mollis sodales condimentum. Etiam dignissim libero nulla, ac imperdiet odio volutpat vel. Sed arcu magna, convallis id lorem convallis, pharetra faucibus libero. Nulla id condimentum sem. Suspendisse neque diam, volutpat eu nulla eu, blandit pulvinar lorem. Nunc efficitur, sem sed metus.";
    payload.assign(str.begin(), str.end());
    compress_test(payload);

    str = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec placerat rhoncus ultricies. Vestibulum sit amet nulla semper ex semper faucibus quis nec lacus. Quisque quis blandit elit. Morbi non iaculis ligula, facilisis dictum odio. Nam accumsan eros a elit tristique, ut suscipit magna lacinia. Mauris porttitor tortor eu metus sollicitudin elementum. Curabitur blandit, ipsum in condimentum facilisis, enim arcu lobortis ligula, ac imperdiet dolor sem in erat. Curabitur porttitor turpis ut amet.";
    payload.assign(str.begin(), str.end());
    compress_test(payload);

    str = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nam convallis tortor a nisi viverra luctus. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Nunc aliquam dui nec gravida vehicula. Fusce sit amet ultricies magna. Phasellus in est a odio pretium rutrum sit amet sed lacus. Etiam vel facilisis massa. Duis ornare, magna in lacinia congue, turpis ipsum imperdiet ante, eget malesuada tortor ipsum eget nisi. Donec dictum lectus at augue consectetur suscipit. Nam quam mi, congue sit amet ultrices quis, blandit sed felis. Donec at tortor vitae nulla condimentum scelerisque.\n"
          "\n"
          "Cras laoreet porttitor tristique. Aliquam dui velit, placerat tincidunt arcu a, posuere vestibulum leo. Aliquam accumsan quam metus.";
    payload.assign(str.begin(), str.end());
    compress_test(payload);

    str = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Fusce porta, lectus non placerat scelerisque, nulla diam vehicula tellus, a molestie eros felis et felis. Aenean nec ultricies arcu. Praesent eget tincidunt lectus. Praesent condimentum diam eget sapien commodo finibus. Nullam fringilla nec arcu iaculis elementum. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nulla eget ipsum nec risus gravida fringilla. Donec ligula augue, viverra non metus sit amet, convallis dapibus est. Curabitur risus est, suscipit eget dapibus sit amet, elementum id risus. Etiam eleifend dolor at scelerisque mattis.\n"
          "\n"
          "Cras pharetra risus purus, in rutrum mauris malesuada vel. Aliquam suscipit leo tellus, et luctus diam viverra ut. Praesent maximus odio quis odio venenatis, id varius risus convallis. Aenean laoreet varius magna ut sagittis. Sed sed convallis mauris. Curabitur eu lorem ligula. In hac habitasse platea dictumst. Nam commodo eros risus, id dapibus dui euismod vel. Donec tempus cras amet.";
    payload.assign(str.begin(), str.end());
    compress_test(payload);

    str = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Vivamus dictum ex vel auctor cursus. Aenean fringilla tristique metus ut accumsan. Duis quis fermentum ipsum. Nullam luctus augue sed orci ullamcorper consectetur tempor gravida ipsum. Nullam consectetur lectus id porta euismod. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. Mauris tempus tincidunt erat, a euismod nulla placerat quis. Proin hendrerit orci eget augue vestibulum, at dignissim nisl gravida. Phasellus semper enim sed massa pretium molestie. Nulla pretium neque orci, a rutrum orci ullamcorper sed. Nullam cursus ante et commodo aliquet. Sed sodales sapien turpis. Fusce mollis orci ligula, sit amet rutrum sem scelerisque eu. Curabitur fermentum elementum leo, vitae porta neque porta efficitur. Maecenas euismod pellentesque tortor, eget laoreet orci. Integer maximus ligula at congue tristique.\n"
          "\n"
          "Cras imperdiet sapien at ex pulvinar, posuere consequat purus dapibus. Vivamus in blandit sapien, a ultrices augue. Mauris lacinia semper lorem et ultrices. In vulputate lacus malesuada, mattis elit sed, dignissim neque. Morbi ac mauris ultricies, scelerisque massa vel, venenatis augue. Vivamus imperdiet ante a leo finibus, quis venenatis sapien tincidunt. Curabitur leo massa, feugiat eu nunc id, consectetur consectetur nibh. Pellentesque ac ante maximus, ultrices arcu eu, dictum odio. Ut lorem orci, condimentum id porta sit amet, vehicula eu est. Pellentesque quis augue amet.";
    payload.assign(str.begin(), str.end());
    compress_test(payload);

    str = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Vestibulum sit amet felis dapibus, pretium dolor eu, posuere magna. Suspendisse ornare sagittis nulla fringilla accumsan. Nam scelerisque arcu massa, a vestibulum massa facilisis sed. Aenean ornare enim ut quam rhoncus, ac gravida nisl commodo. Aliquam vitae viverra lorem. Phasellus vitae lectus cursus, cursus augue eget, tristique risus. Maecenas vitae metus diam. Praesent sit amet cursus tellus. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Maecenas eu odio quis lectus efficitur efficitur. Quisque ut est tristique, luctus quam quis, varius ex.\n"
          "\n"
          "Quisque semper consectetur ipsum. Vestibulum cursus lobortis tellus, ut aliquet quam malesuada nec. Ut tristique libero ut aliquet imperdiet. In sodales, magna nec suscipit iaculis, metus quam volutpat sem, quis commodo diam nibh id massa. Vivamus et ornare diam, non malesuada risus. Nam tincidunt, purus eget vestibulum semper, lacus eros sagittis mauris, eget vestibulum magna nisi a erat. Proin tempor commodo metus volutpat condimentum. Praesent eu nunc at nibh mattis feugiat id eu nibh.\n"
          "\n"
          "Nunc dapibus orci rutrum tellus dapibus mattis id faucibus est. Nunc eleifend libero id consequat gravida. Interdum et malesuada fames ac ante ipsum primis in faucibus. Nullam maximus odio eu molestie finibus. Nunc a iaculis mi. Maecenas ut sollicitudin urna. In velit ipsum, eleifend id tempor non, feugiat non nulla. Cras porta, ex id dictum aliquam, turpis neque ultrices libero, quis tincidunt odio magna in libero. Maecenas pellentesque aliquam urna eu tincidunt. Vestibulum quis magna metus. Nulla lacinia purus nisl, at scelerisque mi venenatis et. Sed dapibus commodo enim at elementum.\n"
          "\n"
          "Vivamus vel diam faucibus, congue ipsum eu, maximus nisl. Nam efficitur dolor non augue aliquet, in pellentesque nulla tristique. Praesent arcu turpis, tincidunt ac porttitor sit amet, iaculis vitae nisi. Vestibulum at justo vitae tortor consectetur elementum. Curabitur ornare nisl massa nunc.";
    payload.assign(str.begin(), str.end());
    compress_test(payload);

    str = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. In euismod justo lorem, vel commodo libero finibus vel. Praesent imperdiet, felis quis sollicitudin aliquam, felis libero lacinia nulla, non fermentum risus lacus vel magna. Nam in neque sit amet orci aliquam aliquet. Nulla varius, est eget luctus accumsan, lorem ex egestas diam, ut pretium eros urna sit amet nunc. Mauris sagittis lobortis metus sit amet hendrerit. Fusce quis sem diam. Etiam eget velit vitae est tempor lobortis. In feugiat pretium erat at vulputate.\n"
          "\n"
          "Phasellus eget enim quis neque laoreet facilisis. Maecenas eget augue non ante efficitur rhoncus. Etiam non lorem ullamcorper nulla luctus tristique. Proin ipsum nisi, congue aliquam quam aliquam, laoreet feugiat purus. In eu sagittis est. In pretium ullamcorper bibendum. Nullam aliquet consectetur iaculis.\n"
          "\n"
          "Proin non fringilla odio. Pellentesque et commodo enim. In vulputate, odio quis tincidunt tincidunt, lectus sem pellentesque velit, et feugiat magna ligula ac erat. Curabitur mauris nulla, commodo et nisl in, ultrices efficitur urna. Nulla quis lorem ultrices, cursus enim eget, tempor augue. Sed lacinia ipsum mi, quis congue sem consectetur eu. Nullam egestas dictum maximus. Mauris tincidunt nisl et mauris ullamcorper commodo. Donec tempus ex aliquam nulla malesuada blandit. Proin molestie lobortis dolor at malesuada. Cras sit amet velit nisl. Cras nec lectus dictum, pulvinar velit vitae, varius purus. Duis non faucibus mi. Cras lacinia velit ante, nec auctor risus viverra sit amet. Nulla egestas viverra orci, sed rutrum neque tincidunt a.\n"
          "\n"
          "Nulla at quam erat. Nulla faucibus sapien a massa euismod dignissim. Nunc libero erat, dictum sed pulvinar in, luctus id enim. Vivamus at sagittis mauris, at accumsan nulla. Nullam non neque non velit consectetur commodo. Vestibulum iaculis, quam eu consectetur molestie, eros turpis vehicula mauris, nec fermentum sapien enim a ex. Interdum et malesuada fames ac ante ipsum primis in faucibus. In et neque eget velit facilisis iaculis in nec mauris. Vivamus urna urna, sollicitudin ac velit at, consequat gravida neque. Suspendisse quis ex ac orci molestie vestibulum ut in mi. Duis mollis nisl eu eleifend tristique. Suspendisse vel dui in sapien dictum sollicitudin. Vivamus at vehicula orci. Aliquam sodales eu nisi id pharetra.\n"
          "\n"
          "Aenean dolor est, elementum ut luctus id, posuere vitae ex. Proin cursus efficitur dui id dapibus. Sed eget vestibulum nibh, laoreet volutpat ligula. Aliquam varius eu mauris vitae venenatis. Donec ac sem nulla. Nam sit amet ex urna. Phasellus erat velit, fermentum blandit molestie quis, imperdiet vel libero.\n"
          "\n"
          "Maecenas vitae odio vel urna auctor rutrum sed a lacus. Curabitur non lorem ut turpis dictum egestas in sed enim. Fusce non sem in risus consectetur tempor. Quisque egestas magna odio, eget placerat tellus consequat facilisis. Etiam faucibus sodales metus vel bibendum. Donec dui lectus, fermentum eu diam sit amet, luctus ultricies ante. Ut semper aliquet purus, ac viverra dui molestie ac. Nullam tincidunt elit tellus, vel pharetra elit pharetra ut.\n"
          "\n"
          "Donec condimentum risus non enim fringilla, mollis laoreet ligula luctus. Aliquam nec malesuada arcu, in molestie ante. Pellentesque quis tellus eu est interdum suscipit quis quis ipsum. Vivamus lorem ipsum, tristique in velit a, luctus aliquam lorem. Maecenas accumsan orci dignissim hendrerit volutpat. Duis volutpat odio nec est tempor, a tempus massa gravida. Vivamus porta ligula ornare ipsum porttitor, nec interdum enim malesuada. Pellentesque vitae sapien dictum ex imperdiet congue. Proin aliquam lectus dui, eu tincidunt ipsum porttitor interdum. Duis nulla tellus, congue eget maximus et, malesuada et velit. Orci varius natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Vivamus ut est vel dolor varius rhoncus eget non felis. Sed eget nisl euismod, pharetra enim eget, facilisis ligula.\n"
          "\n"
          "Quisque odio lacus, pellentesque eget suscipit at, pulvinar et nulla. Nulla viverra sed nisi in egestas. Aenean consectetur aliquet metus eu consequat. Vivamus pharetra, nibh at aliquet luctus, ipsum neque interdum nunc, eget tempus libero dolor ut neque. Nullam laoreet magna turpis, vitae condimentum ante viverra sed. Integer in orci finibus, interdum eros id, rhoncus purus. Quisque id risus velit. Aliquam sed velit commodo, auctor metus ac, consectetur risus. Donec ullamcorper pulvinar orci aliquam ultricies. Morbi posuere mi lacus, tincidunt porta eros congue eu. Mauris magna lacus, blandit vitae dignissim eget, porta quis mi. Curabitur tristique dui at laoreet vestibulum. Quisque fermentum tellus et ultricies laoreet. Nulla at malesuada mi. Quisque ut tempus enim.\n"
          "\n"
          "Cras lobortis lacinia turpis ut sodales. In sapien odio, congue sit amet risus sed, tincidunt fermentum est. Ut tincidunt finibus augue a condimentum. Vestibulum laoreet, lectus id vestibulum tincidunt, metus erat dictum lorem, eget accumsan est purus eu nibh. Maecenas ac dolor sit amet quam efficitur faucibus. Sed sed.";
    payload.assign(str.begin(), str.end());
    compress_test(payload);

    str = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nam imperdiet a felis vitae aliquet. Nam in ultrices risus. Integer condimentum consectetur lorem ultricies condimentum. In at gravida diam. Curabitur pretium sit amet sem non congue. Praesent aliquam dictum dolor sit amet luctus. Nulla condimentum ultrices nibh eu dignissim. Nulla tempor, mauris nec bibendum gravida, leo sapien bibendum mauris, a consectetur ligula libero ut massa. Morbi rhoncus elementum laoreet. Integer aliquam, sem eu fringilla commodo, dui eros eleifend odio, nec lobortis velit magna fermentum justo. Praesent eget elementum quam, id mattis orci. Nullam convallis quis orci ut molestie. Nam volutpat sit amet diam at ullamcorper. Nulla commodo iaculis semper. Cras vitae diam sagittis sem pharetra dapibus varius nec est.\n"
          "\n"
          "Nam placerat dapibus lectus a dapibus. Curabitur in magna ac nisi finibus pretium. Nunc eu faucibus felis. Vivamus lacus lectus, dignissim scelerisque leo ut, maximus posuere augue. Praesent euismod eu leo sed suscipit. Phasellus eget finibus erat. Vestibulum placerat velit sed metus sodales accumsan. Pellentesque bibendum, justo sit amet mollis faucibus, tellus dui eleifend orci, nec vehicula ipsum diam non nunc. Aliquam accumsan nisl sollicitudin felis efficitur venenatis. Duis ullamcorper nisl sit amet suscipit tristique. Orci varius natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Morbi sodales ligula sem, eu fermentum ligula tempor a.\n"
          "\n"
          "Fusce aliquam turpis massa, in laoreet dui sodales vitae. Sed elementum elit lectus. Suspendisse potenti. Etiam vel egestas orci, quis sagittis eros. Praesent auctor rhoncus velit, ac condimentum lorem accumsan eu. Fusce et tellus iaculis, suscipit neque malesuada, consectetur sapien. Fusce lacus nulla, tincidunt vitae pretium ac, facilisis id massa. Aliquam venenatis velit ut lobortis aliquam. Class aptent taciti sociosqu ad litora torquent per conubia nostra, per inceptos himenaeos. Sed enim tellus, tristique nec venenatis a, suscipit tincidunt magna. Fusce ac arcu lacinia, molestie est venenatis, dapibus dui. Ut in accumsan ligula, ac lacinia nulla. Nulla lectus lacus, bibendum sed iaculis non, ultrices pharetra justo. Sed condimentum, sem quis efficitur aliquam, lectus erat condimentum nunc, in feugiat sapien ex eu eros.\n"
          "\n"
          "Suspendisse ullamcorper id augue vitae accumsan. Quisque quis porttitor est. Phasellus elementum urna quis urna faucibus tristique. Mauris venenatis purus lectus, et sollicitudin nisi fermentum eu. Mauris pretium placerat turpis, sit amet lobortis ligula efficitur ac. Proin sed congue magna. Quisque felis purus, sollicitudin sit amet viverra nec, posuere et odio. Morbi lacinia ante id diam sollicitudin ultrices. Curabitur sollicitudin felis id felis tempor aliquet. Morbi sit amet enim nec sem laoreet pretium sit amet vitae magna.\n"
          "\n"
          "Phasellus ut hendrerit dui. Nulla dapibus turpis leo, suscipit varius velit sodales non. Integer quis nibh non eros vehicula laoreet et nec lorem. Curabitur fermentum arcu sit amet ex lobortis, sollicitudin vestibulum arcu ultrices. In hac habitasse platea dictumst. Maecenas tincidunt accumsan elit eget fringilla. Nam sed ipsum vitae nisi porttitor aliquet et a orci. Praesent ut molestie nulla. Suspendisse blandit cursus nisl, vel vestibulum tellus. Sed non sodales ligula. Morbi suscipit viverra metus at ultrices. Donec rutrum nisl quis augue placerat suscipit. Ut velit metus, sollicitudin a purus at, consequat viverra lorem. Donec vel ligula felis. Phasellus blandit lacinia congue. Phasellus consequat sapien ullamcorper erat interdum ultrices.\n"
          "\n"
          "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nulla facilisi. Donec rhoncus orci id feugiat varius. Ut ullamcorper orci non dui volutpat suscipit. Donec justo velit, vehicula id convallis nec, sollicitudin et mi. Integer eu orci eget sem malesuada iaculis. Aenean vulputate nunc nec tincidunt commodo. Vivamus ultricies libero dui, sit amet auctor augue bibendum nec. Nunc varius porttitor velit eget auctor. Fusce in felis et metus vehicula vehicula. Pellentesque pellentesque diam ac erat ultricies tincidunt. Pellentesque sit amet eros gravida ipsum mattis sagittis.\n"
          "\n"
          "Sed blandit tempus velit, in imperdiet felis bibendum eu. In ac tempus neque. Donec in leo quis nulla vestibulum posuere vel ac nibh. Suspendisse leo ante, volutpat et lorem sit amet, dapibus aliquet augue. Duis pretium tempor egestas. Cras sit amet neque pharetra, bibendum nisl malesuada, tincidunt odio. Sed eget ipsum dolor. Ut in malesuada neque, non molestie mi. Curabitur sed tempus lorem. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Duis non hendrerit enim. In sagittis gravida dolor in tempor. Curabitur vulputate magna magna, id gravida sem placerat nec.\n"
          "\n"
          "Nullam posuere sodales purus, accumsan venenatis nibh egestas ut. Morbi non condimentum mi. Nam nulla purus, ultricies non ante quis, facilisis vestibulum mauris. Ut non viverra dolor. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Morbi imperdiet vel metus in congue. Vivamus bibendum risus ligula, vel malesuada justo mollis et. Fusce nec magna rhoncus nisl hendrerit scelerisque. Ut feugiat gravida mauris, id aliquam neque eleifend eu. Proin consectetur lectus ac tortor efficitur, vitae luctus arcu finibus. Sed vulputate finibus molestie. Donec vitae orci nisi. Vestibulum a nibh quis purus sodales eleifend. Ut interdum, quam quis tincidunt vestibulum, mauris magna cursus nulla, a faucibus mi est vel sem.\n"
          "\n"
          "Curabitur eu finibus arcu, in laoreet tellus. Duis quis ultricies leo. Vestibulum id justo id neque iaculis vulputate sit amet vel libero. Quisque at urna finibus, auctor quam quis, accumsan massa. Praesent luctus ante non rhoncus sollicitudin. Cras et urna ultricies, auctor mauris sed, dignissim tortor. Ut a neque sed lectus efficitur commodo ut quis augue. Maecenas viverra auctor diam at venenatis. Curabitur aliquam sit amet sem ac egestas. In varius ante neque, a ornare mi tempor volutpat. Sed luctus lacinia justo eget egestas. Suspendisse eget sollicitudin nisl. Sed dignissim sed leo sed varius. Nam id odio eget augue cursus tempor ut a nulla.\n"
          "\n"
          "Vivamus lacus justo, dictum viverra lorem tincidunt, semper eleifend odio. Proin non nisl at velit venenatis consectetur. Nam malesuada lacus massa, ut hendrerit nulla placerat nec. Duis eget sapien urna. Nunc non sollicitudin magna. Quisque porta, ante a tincidunt luctus, leo magna porta lacus, in tempus felis massa et mi. Nullam quis luctus neque. Integer tempor, arcu vel varius congue, nisl ex sodales ante, a mollis massa orci eget quam. Quisque elementum massa quis nibh blandit egestas. Donec purus turpis, lacinia nec varius sit amet, placerat sit amet massa. Nullam ac ultricies odio, molestie consequat mauris. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Duis condimentum massa lacus, id tincidunt lacus semper id. Vivamus tincidunt sodales euismod. Nulla ac dictum metus, sed bibendum sapien. Lorem ipsum dolor sit amet, consectetur adipiscing elit.\n"
          "\n"
          "Donec quis nisl diam. Pellentesque ut sollicitudin massa. Integer ac neque porttitor, mattis dui quis, tincidunt purus. Morbi lectus leo, ornare in bibendum ut, molestie non mauris. Nunc sed posuere tellus. Nullam eget tellus mollis, faucibus ipsum sit amet, ullamcorper odio. Ut non dui laoreet, congue velit ac, finibus augue. Praesent sollicitudin tellus ac placerat efficitur. Proin vel tellus vestibulum, luctus eros quis, laoreet urna. Etiam nec justo ligula. Nulla eget condimentum ante. Proin lacinia erat lacus, id tristique neque vestibulum at. Nulla facilisi. Integer et efficitur orci, volutpat pulvinar ante. Vestibulum id venenatis velit, eu euismod arcu.\n"
          "\n"
          "Vivamus consectetur mattis tincidunt. Duis placerat rhoncus ipsum, nec venenatis purus sollicitudin sit amet. Aliquam dui diam, laoreet in venenatis eget, imperdiet nec nibh. Aenean commodo, ex sed consequat blandit, odio risus mollis leo, eu maximus erat mi sed ante. Nam id orci dolor. Curabitur dignissim nisi et rhoncus aliquam. Donec sit amet odio ultricies, dapibus urna nec, tempus nulla. Nulla facilisi. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Suspendisse quam leo, auctor vitae sollicitudin ac, gravida non lorem. Duis dapibus urna eu arcu pretium ornare. Sed ornare convallis urna quis porta. Sed aliquet fringilla lorem vel ornare. Phasellus orci libero, tempor ut augue in, mollis mattis sem. Sed iaculis vitae neque vel bibendum. In sit amet efficitur purus, at pharetra elit.\n"
          "\n"
          "In sagittis maximus nunc eget aliquam. Nunc egestas ac erat at cursus. Vivamus at ipsum vel erat aliquam fermentum. Cras finibus molestie augue vitae viverra. Praesent tincidunt metus sapien, sodales dapibus elit hendrerit in. Interdum et malesuada fames ac ante ipsum primis in faucibus. In ac risus in tellus venenatis euismod eu non turpis. Ut mattis, ante sed consectetur ornare, est nunc luctus ante, ut pulvinar orci tortor dictum libero. Suspendisse sit amet orci sed mi eleifend rhoncus non vel risus. Curabitur dictum lectus est, et ullamcorper enim pharetra nec. Curabitur tempus eros finibus, blandit lacus non, molestie quam. Fusce tincidunt dictum massa finibus placerat. Integer et justo justo. Aliquam commodo diam augue. Maecenas nunc erat, feugiat vel sagittis id, semper nec ligula.\n"
          "\n"
          "Suspendisse ut laoreet erat, in dignissim lectus. Nulla ipsum libero, interdum sed pharetra vel, egestas at nunc. Nam dictum consectetur ligula, a commodo ante suscipit vel. Praesent vulputate sit amet nulla non pretium. Integer eget pellentesque ligula, rhoncus tristique lacus. Vivamus quis turpis nec arcu finibus laoreet. Ut interdum rutrum sodales. Nam fermentum velit quis quam viverra euismod. In mattis diam ex, quis ornare nulla commodo nec. In sed nunc euismod, pretium tellus bibendum, congue ante. Nullam dolor justo, porta at iaculis vitae volutpat.";
    payload.assign(str.begin(), str.end());
    compress_test(payload);

    str = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Mauris elementum ac neque nec ornare. Aenean rutrum sem diam, eget euismod orci faucibus ac. Etiam eu tempus neque, ut lacinia leo. Duis ut mollis sapien. Duis sit amet sollicitudin augue. Nullam tellus metus, consequat sit amet libero eu, aliquet suscipit nisl. Maecenas tincidunt molestie interdum. Aliquam consequat lectus vitae efficitur porttitor. Duis et velit dictum, posuere dolor congue, dapibus massa. Sed ultricies, libero id ultricies condimentum, tortor massa iaculis urna, eget mollis nibh magna id tortor.\n"
          "\n"
          "Etiam fermentum egestas nunc, a ornare felis tincidunt non. Nulla vel maximus arcu. Sed id maximus nibh, eu rhoncus arcu. In sapien lacus, lacinia a sollicitudin sodales, tincidunt vel diam. In in urna eu lectus placerat vulputate vel vitae nisl. Aenean vitae tellus non libero pharetra laoreet. In placerat scelerisque sem a ullamcorper. Maecenas ac augue nec erat efficitur pellentesque. Praesent molestie odio et tellus blandit sodales. Aliquam posuere justo a erat dignissim, ut porta odio ultrices. Nulla diam quam, hendrerit eu cursus nec, mollis vitae sem. Proin feugiat, dui quis blandit semper, nisi dui aliquam leo, sed sagittis mauris sem nec nisi. Nullam tempor imperdiet venenatis. Etiam non arcu eu lacus interdum tempor non nec lectus. Sed quis tempus nisl, eu tempus magna.\n"
          "\n"
          "In eu ante ac tortor pellentesque commodo at et nulla. Mauris ut lectus dui. Nam lacus felis, tempor a diam ut, blandit vestibulum ligula. Ut eget quam et libero pulvinar ultricies. Praesent facilisis tortor eu odio eleifend, vel finibus ante malesuada. Vestibulum venenatis ac libero id commodo. Orci varius natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Proin ornare nibh vel tempor venenatis. Donec id tellus justo. Aenean efficitur ex diam, non iaculis dui pharetra ac. Cras venenatis lobortis augue, in tempus dui accumsan non. Nullam eleifend a tortor quis porttitor. Duis rhoncus justo ipsum, et fermentum neque pulvinar id. Integer eget elementum tortor. Vestibulum a dapibus nibh, tincidunt lobortis leo. Ut mattis libero a lacus lacinia interdum.\n"
          "\n"
          "Suspendisse convallis leo at nulla ultrices bibendum. Aliquam arcu est, suscipit sit amet elit sit amet, dictum condimentum arcu. Maecenas nec dui ultrices, aliquam augue vitae, suscipit leo. Nunc laoreet lectus sed viverra lacinia. Curabitur sagittis dictum arcu, sed interdum neque condimentum a. Donec finibus mauris vel aliquet blandit. In tempus vulputate egestas.\n"
          "\n"
          "Etiam malesuada euismod nibh a accumsan. Donec maximus elementum nunc porttitor auctor. Sed lobortis sit amet est ut sodales. Vestibulum lorem quam, viverra a lobortis eu, vestibulum eu turpis. Nulla eu orci quam. Fusce lobortis tempor ipsum id fringilla. Ut placerat neque ac ligula semper vulputate. Proin eget tortor nunc. Nulla eu mauris et eros condimentum iaculis. Donec id orci blandit, vulputate quam quis, scelerisque arcu. Pellentesque vitae orci eu risus sodales sollicitudin. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nam dictum quam sem, eu cursus dui hendrerit efficitur. Sed pretium, mauris nec sodales dictum, urna est rutrum dui, sit amet maximus nisi nulla et dui. Integer fringilla vehicula enim in dictum.\n"
          "\n"
          "Aliquam erat volutpat. Proin lacus nisi, dignissim vulputate risus et, facilisis gravida neque. Nulla pulvinar erat nisl, at vehicula ligula efficitur interdum. Etiam id neque accumsan, faucibus massa a, euismod felis. Donec sodales luctus elit a tincidunt. Integer vulputate ac lectus a tempus. Aenean vitae dui congue, varius eros et, facilisis sem.\n"
          "\n"
          "Vestibulum tincidunt finibus erat quis lacinia. Nunc maximus tellus ut urna tristique, vel dignissim erat pharetra. Suspendisse lectus tortor, sodales nec rutrum nec, accumsan vel risus. Etiam quis porta tellus. Cras eget viverra lacus, id malesuada purus. Curabitur nec metus at ipsum faucibus luctus ac at quam. Mauris semper orci sed eros laoreet dignissim vel eu sem. Donec vel consequat lacus.\n"
          "\n"
          "Integer viverra venenatis eleifend. Maecenas metus elit, consequat sed sapien non, scelerisque lacinia elit. Integer et purus eu risus interdum ornare in et sem. Curabitur at aliquet odio. Donec ultricies enim quis urna lacinia, in tincidunt justo ornare. Sed posuere risus orci, vulputate volutpat nibh fermentum ut. Vivamus nisl orci, pharetra in erat vel, aliquet accumsan velit. Sed eget turpis ut risus volutpat tristique. Proin pharetra interdum enim, quis cursus nisl volutpat quis. Integer sagittis risus leo, a efficitur ex sagittis non.\n"
          "\n"
          "Nam vel sem nec lorem viverra tempor eget iaculis lacus. Aenean sed vestibulum neque, at condimentum nulla. Phasellus laoreet sed lectus in tincidunt. Class aptent taciti sociosqu ad litora torquent per conubia nostra, per inceptos himenaeos. Ut rhoncus mi vitae nisl finibus convallis. In ut lorem felis. Mauris molestie quis lorem suscipit efficitur. Nunc auctor sapien vel tincidunt pharetra. Nam ultrices imperdiet diam, eu aliquet dui pulvinar sit amet. Nunc neque velit, egestas a ex ut, mattis pretium libero. Nulla facilisi. Praesent condimentum nibh tellus, ut semper diam aliquam id. Etiam maximus turpis non nibh lacinia dictum. Quisque elementum est nec magna faucibus pretium. Integer malesuada euismod ligula, sit amet pretium felis feugiat eget.\n"
          "\n"
          "Ut a convallis tortor. Mauris porttitor, ligula nec efficitur convallis, lectus lacus commodo lorem, sed dapibus mi ex a purus. Aliquam et ultrices nunc, eget laoreet lacus. Duis tellus diam, sollicitudin sit amet gravida et, condimentum eu ligula. Nulla ornare velit ante, vel fringilla tortor vehicula vel. Aliquam ultricies tortor et facilisis ultricies. Etiam vitae consequat est, at fermentum nibh. Nunc fringilla urna at ornare lobortis. Vivamus quis consectetur arcu, eget vehicula nulla. Morbi tincidunt massa massa, vel condimentum est luctus eu.\n"
          "\n"
          "Curabitur eu libero ac urna bibendum porttitor. Praesent condimentum, lorem et blandit luctus, dolor felis sollicitudin metus, consectetur ullamcorper odio justo vitae erat. Maecenas at magna diam. Suspendisse mattis volutpat magna ut fringilla. Sed tellus nisi, interdum ut dui et, tempus maximus urna. Maecenas eget varius lacus. Aliquam felis risus, aliquam in ultricies sit amet, gravida sed libero. Suspendisse potenti.\n"
          "\n"
          "Vestibulum et tempus nisi, ac euismod mauris. Curabitur tincidunt nisl in consectetur hendrerit. Fusce condimentum mauris eget nisi dapibus, id pulvinar ex sollicitudin. Donec molestie facilisis egestas. Sed pulvinar, nibh et hendrerit eleifend, mi sem tempor dui, a molestie neque metus vel diam. Phasellus mauris sapien, auctor vestibulum fringilla pulvinar, egestas quis leo. Nam quis vulputate massa, in finibus neque. Morbi sagittis dui lorem, et rhoncus ligula mollis at. Vestibulum scelerisque neque porttitor leo pharetra pulvinar.\n"
          "\n"
          "Nullam eget tempus massa. In in commodo velit, nec sagittis metus. Aenean ligula ligula, posuere nec cursus quis, mollis vel mi. Sed ultrices nisl sit amet nisl euismod, semper interdum risus pharetra. Proin massa magna, feugiat sed massa sit amet, iaculis pellentesque eros. Donec ultrices venenatis molestie. Aenean congue felis ligula, id pretium mauris tincidunt eu. Vestibulum eget facilisis massa. Nullam aliquet felis eu volutpat gravida. Aenean tellus sapien, tempor vitae luctus a, imperdiet id nunc. Nunc sagittis nunc at vulputate maximus. Sed nulla metus, sagittis non mauris vitae, pulvinar dapibus mauris.\n"
          "\n"
          "Quisque luctus leo in massa vehicula dictum. Etiam pulvinar pretium bibendum. Pellentesque eleifend euismod viverra. Nam elementum auctor rutrum. Praesent sodales fringilla odio, ac ultrices lectus semper at. Nullam sed dolor libero. In quis elit auctor, hendrerit mi at, tincidunt turpis.\n"
          "\n"
          "Nunc vitae pellentesque quam, nec pulvinar nulla. Pellentesque scelerisque consectetur fringilla. Donec consectetur, nulla nec scelerisque iaculis, eros quam posuere nisl, vel dictum massa urna quis augue. Mauris elementum quam vel enim viverra aliquet. Aliquam commodo mi sed lacinia dapibus. Sed aliquam nibh pretium, porttitor orci quis, ornare justo. Donec eget nulla sapien. Integer id enim vitae nibh aliquet gravida id eu erat. Praesent tristique nibh turpis, a mollis erat fringilla eget. Nulla luctus dolor ut eros fermentum, quis semper est ultrices.\n"
          "\n"
          "Nam diam neque, ornare id sodales id, volutpat placerat augue. Sed hendrerit vel nisi id tempus. Sed ipsum augue, aliquam sit amet nunc in, imperdiet feugiat nibh. Duis consequat nulla quis neque bibendum, a iaculis erat convallis. Aliquam dolor tortor, congue a convallis vitae, rhoncus et nulla. Donec consequat sed lorem volutpat mollis. Vivamus bibendum at dui quis luctus. Nunc elementum ultricies vestibulum. Nam in justo lorem. Mauris eu dictum libero. Suspendisse vitae pulvinar eros. Donec fringilla, lorem ut malesuada maximus, metus metus euismod elit, pretium viverra nulla erat pretium leo. Aliquam erat volutpat. In ornare felis ut diam volutpat, id molestie dolor vulputate.\n"
          "\n"
          "Quisque ut nisl fringilla massa consequat euismod eu vitae tellus. Curabitur ut condimentum ipsum. Curabitur nec mi sed nibh consequat sodales vitae vitae nisi. Morbi vel eros quis metus efficitur mattis. Curabitur vitae lacus id felis consequat mollis. Duis augue est, interdum id mi ut, bibendum fermentum sem. Pellentesque gravida placerat purus nec iaculis.\n"
          "\n"
          "Sed lobortis lacus non luctus aliquet. Vestibulum felis arcu, sollicitudin nec interdum et, rhoncus et sem. Suspendisse eu purus in orci consectetur ullamcorper. Nam ac urna sit amet felis varius rhoncus at vel velit. Curabitur volutpat dignissim dolor at blandit. Nam sed placerat nunc, at feugiat lectus. Fusce ac eros sit amet risus ullamcorper egestas. Integer volutpat purus vel laoreet tempus. In hendrerit ante nec nunc tincidunt, cursus iaculis justo rutrum. Maecenas consectetur nisl id purus venenatis dapibus. Integer vel ante sit amet arcu hendrerit molestie sed posuere mauris.\n"
          "\n"
          "Ut sollicitudin nisl at accumsan placerat. Sed tempor, eros nec tempor elementum, ex libero imperdiet nisl, eget posuere nibh orci at nibh. Nullam non faucibus neque. Donec quis scelerisque justo. Cras odio mi, porttitor sit amet enim non, luctus egestas ante. Nunc a leo purus. Nulla non turpis mattis, laoreet nunc vestibulum, interdum justo. Quisque pellentesque rhoncus dui, nec ullamcorper mi.\n"
          "\n"
          "Morbi finibus vehicula nisl vel scelerisque. Integer leo erat, rhoncus fringilla elit pulvinar, blandit accumsan lacus. In vulputate purus vel nisi tincidunt, eu iaculis enim porttitor. Aliquam vulputate faucibus elementum. Suspendisse egestas enim mi, at feugiat ligula feugiat in. Aenean nulla ipsum, tincidunt eget dolor suscipit, luctus viverra augue. Fusce vitae laoreet ante, cursus viverra enim. Aliquam laoreet ipsum sem, eu tempor sapien faucibus sed. Morbi vulputate diam non aliquet venenatis. Morbi fringilla enim at arcu laoreet dictum. Etiam dictum a odio sit amet ultrices. Donec in diam viverra, euismod tortor in, suscipit nulla.\n"
          "\n"
          "Suspendisse potenti. Quisque vitae lobortis lectus. Pellentesque dui eros, molestie in eleifend vitae, scelerisque eget sem. In vitae libero elit. Phasellus aliquet malesuada lectus, non ultrices odio viverra non. Morbi tortor sem, sollicitudin eget sem id, porttitor gravida lacus. Praesent cursus turpis in ante eleifend scelerisque. Fusce semper velit massa, at interdum erat iaculis quis. Donec felis odio, suscipit vel arcu quis, sodales tempus eros.\n"
          "\n"
          "Nullam accumsan elit in finibus aliquam. Quisque quis est ac lectus hendrerit posuere ac id arcu. Aenean ac nunc porttitor, rutrum nibh volutpat, iaculis dui. Phasellus vulputate est volutpat lacus pellentesque, et vestibulum erat imperdiet. Mauris id lorem in orci rutrum dictum. Vestibulum aliquam libero sit amet ex mattis lacinia. Pellentesque non nisl sollicitudin, sodales justo in, volutpat odio. Sed molestie ipsum mauris, nec commodo velit egestas sit amet. Nunc pretium dolor ut tortor elementum, vel vestibulum ipsum tincidunt. Donec eleifend, purus ut luctus eleifend, neque neque fermentum est, sit amet euismod turpis nulla eget ipsum. Interdum et malesuada fames ac ante ipsum primis in faucibus. Ut non ipsum a massa vulputate molestie. Duis ac elit odio. Quisque porta quam vel lectus scelerisque scelerisque. Integer nisl odio, varius vitae felis et, cursus scelerisque nisi. Quisque vitae lacus cursus, aliquam tortor vitae, auctor dui.\n"
          "\n"
          "Curabitur id enim ac purus vehicula dictum. Proin vitae sem porta, viverra tortor id, euismod purus. Vestibulum lorem lectus, fringilla quis sem in, rutrum tempor quam. Praesent vehicula ante sit amet est bibendum, at facilisis sapien dignissim. Morbi quis enim est. Donec interdum fringilla ex, vitae bibendum risus congue a. Fusce congue justo sit amet aliquam ullamcorper. Sed sit amet fringilla nisi. Vestibulum eget vehicula est. Vestibulum vulputate lorem et metus sodales hendrerit. Curabitur vel dui eros. Integer sit amet arcu vel elit pharetra ornare.\n"
          "\n"
          "Nunc molestie congue sapien eget semper. Aenean mi ex, lacinia vel consequat at, placerat sit amet dui. Morbi pretium pretium ipsum, quis commodo sapien iaculis et. Maecenas in est viverra, ultricies dui a, accumsan ipsum. Duis magna sapien, pulvinar at lacus et, sagittis rhoncus mauris. Orci varius natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Cras et mattis purus. Praesent purus dui, varius eu auctor pellentesque, facilisis quis arcu. Suspendisse sodales gravida justo, eget semper leo blandit ut. Suspendisse dictum tellus erat, vitae sagittis libero placerat ut. Integer lacinia luctus odio, at malesuada dolor rutrum a.\n"
          "\n"
          "Sed interdum eget ligula at tincidunt. Phasellus at arcu eget nunc consectetur venenatis. Curabitur ac metus vel metus pharetra congue. Praesent sit amet risus urna. Integer quis convallis dolor. Sed libero nisi, ornare ut varius eget, elementum ut risus. Etiam imperdiet imperdiet porta.\n"
          "\n"
          "Donec cursus neque ac ullamcorper euismod. Etiam ornare, erat nec faucibus malesuada, libero felis rutrum ex, at lacinia tellus mauris accumsan orci. Vivamus et suscipit quam, vitae convallis tortor. Integer eget semper arcu. Etiam fringilla congue justo in finibus. Quisque vitae elementum tortor. Vestibulum at nisl vitae elit pretium fringilla sed ut massa. Ut a justo nisl.\n"
          "\n"
          "Class aptent taciti sociosqu ad litora torquent per conubia nostra, per inceptos himenaeos. Quisque vehicula eros tellus, at vehicula leo faucibus et. Aenean aliquam bibendum congue. Integer feugiat sem diam, et interdum velit hendrerit ut. Vestibulum nisi nunc, placerat et sem ultrices, vestibulum semper lectus. Vestibulum ultrices magna turpis, id posuere risus tempus sed. Praesent eget mauris tellus. Donec id dignissim augue, vitae lobortis augue. Donec sit amet nulla vitae purus ullamcorper rhoncus. Donec libero nulla, ullamcorper vel pellentesque id, commodo vel ipsum. Donec molestie venenatis egestas.\n"
          "\n"
          "Aliquam sed efficitur urna. Suspendisse augue est, imperdiet vitae enim semper, luctus commodo mi. Cras vel lorem lacinia, accumsan neque ac, pharetra orci. Sed fringilla quis eros vitae tincidunt. Integer at tristique mi, eu scelerisque purus. Curabitur ac nisi sed dui viverra tristique. Suspendisse dignissim tincidunt pretium. Sed lectus ex, accumsan vitae ante faucibus, elementum bibendum orci. Aenean faucibus, nisl id tempor vulputate, massa lectus aliquam eros, vel egestas ante augue quis tellus. Quisque convallis, erat vel tempor laoreet, erat quam convallis odio, interdum condimentum elit neque sit amet sem. Donec tellus turpis, mattis ut bibendum at, gravida in purus.\n"
          "\n"
          "Maecenas condimentum pretium ultricies. Nulla facilisi. In elementum ante mi, ut finibus elit tempus sed. Mauris vulputate, neque et ornare viverra, mauris nulla consectetur sem, non ornare nulla sapien eget ex. Proin ullamcorper aliquet odio nec feugiat. Suspendisse feugiat, ante in tristique ultricies, nibh urna blandit enim, vel cursus erat turpis a est. Nunc eu arcu sed lacus convallis mattis. Mauris id nulla iaculis, cursus diam ac, ultricies ligula. In consectetur sapien in lacus elementum porta. Aenean tristique interdum nunc, et faucibus dolor tempus et. Sed ut porttitor urna, molestie euismod enim. Suspendisse molestie placerat libero sit amet iaculis.\n"
          "\n"
          "Vestibulum eros lacus, molestie non pellentesque ut, imperdiet vel velit. Integer euismod fringilla purus, quis aliquet purus consequat ut. Aenean scelerisque ut magna eget luctus. Nulla maximus dictum neque, vel elementum urna. Interdum et malesuada fames ac ante ipsum primis in faucibus. Nam mollis iaculis mauris vitae tristique. Proin faucibus ultrices ipsum, nec tristique mi lacinia at. Pellentesque at leo purus. Integer non justo nec purus tincidunt consequat et non nunc. Nulla sollicitudin rutrum velit, vitae porttitor ligula rhoncus non. Donec laoreet rutrum ipsum eget suscipit. Cras lacinia tellus sit amet libero congue maximus.\n"
          "\n"
          "Sed lorem leo, scelerisque vitae quam ut, facilisis volutpat sem. Mauris vestibulum interdum massa, nec euismod dolor lobortis in. Integer vel augue quis libero molestie convallis. Sed ligula lectus, ultrices quis euismod sit amet, molestie non purus. In ullamcorper imperdiet condimentum. In varius risus risus, at hendrerit leo imperdiet ut. Integer sit amet ipsum tristique odio tempus tincidunt non eget leo. Integer accumsan, ligula at fringilla interdum, lacus orci suscipit enim, at commodo sem nisi a lectus. Fusce neque ante, dictum ut finibus non, aliquam non sem. Maecenas in ex nec elit luctus efficitur ut ut eros. Morbi pellentesque metus sit amet sapien fermentum, quis hendrerit ipsum finibus.\n"
          "\n"
          "Vivamus ullamcorper mauris in urna accumsan, et tincidunt tellus aliquet. Nullam sagittis convallis molestie. Sed faucibus eleifend massa. Nullam fringilla erat sed condimentum mollis. Nullam a auctor enim. Cras faucibus laoreet ante, vitae varius massa lacinia at. Praesent blandit interdum magna ac fermentum. Duis ut velit id elit placerat faucibus. Nunc iaculis, odio vitae faucibus consectetur, felis velit lacinia ex, sit amet hendrerit turpis mi vel diam. Vestibulum nec imperdiet nisi. Mauris molestie rutrum blandit. Etiam vitae leo libero.\n"
          "\n"
          "Sed tristique diam eget mi ornare sodales. Quisque ultrices sit amet eros interdum iaculis. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. Pellentesque dignissim sapien eget congue commodo. Etiam eleifend sodales lobortis. Ut a rutrum ligula. Nunc dignissim elit sed efficitur auctor. Integer sit amet felis orci. Cras in odio nibh. Curabitur maximus sagittis ante, ac accumsan lacus tristique sed. Duis tempor mi eu est consequat, ut efficitur lectus consequat. Morbi eu ipsum porta, lacinia dui eu, hendrerit diam. Integer ipsum ipsum, lacinia egestas pellentesque tristique, eleifend sed risus. Phasellus metus odio, accumsan ut turpis ut, mollis auctor nulla.\n"
          "\n"
          "Proin pretium, nisl ac aliquam finibus, eros nunc aliquam arcu, sit amet ultricies nisi ex vitae nunc. Proin imperdiet, nisl vitae dignissim consequat, tortor tellus ullamcorper sapien, ut gravida arcu magna ac est. Phasellus vel quam malesuada, dapibus ante et, gravida mi. In laoreet nunc quis aliquet suscipit. Cras cursus erat vitae urna tincidunt, at faucibus augue faucibus. Praesent fermentum tincidunt risus, a feugiat tellus convallis iaculis. Donec aliquam elit vitae lorem pellentesque sagittis. Phasellus semper imperdiet lorem, a blandit ipsum condimentum id. Suspendisse non dolor hendrerit, lacinia velit id, imperdiet ligula. Morbi eleifend enim lacus, in pretium sem maximus vel. Praesent nec elementum odio, ut bibendum nisi. Duis metus metus, porta vel eros pharetra, interdum auctor risus.\n"
          "\n"
          "Nam volutpat elit at tellus fringilla, vitae euismod velit laoreet. Aliquam erat volutpat. Nulla pulvinar auctor lacus a vehicula. Proin nec ante accumsan, pellentesque neque at, accumsan velit. Phasellus aliquet lectus arcu, quis sollicitudin lacus blandit quis. Proin venenatis porta libero id ullamcorper. Etiam et vulputate nullam.";
    payload.assign(str.begin(), str.end());
    compress_test(payload);

    return 0;
}



