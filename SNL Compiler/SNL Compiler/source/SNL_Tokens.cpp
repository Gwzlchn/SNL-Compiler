#include"SNL_Tokens.h"
#include<algorithm>



template <typename T, typename F>
const  std::map<T, F> reserveMap(const std::map<F, T>& to_res) {
    std::map<T, F> ret;
    for (auto iter = to_res.begin();
        iter != to_res.end(); iter++)
        ret.insert({ iter->second,iter->first });

    return ret;
}
const std::map<SNL_TOKEN_TYPE, std::string> Token_Type_Name_Map = reserveMap<SNL_TOKEN_TYPE, std::string>(Token_Name_Type_Map);




#define TERMIN true
#define NOTTER false
template<typename T, typename F>
const std::map<F, bool> make_terminal_map(std::map<T, F> full_map) {
    std::map<F, bool> ret;
    for (auto iter = full_map.begin(); \
        iter != full_map.end(); iter++) {

        if (std::all_of(iter->first.begin(), iter->first.end(), [](unsigned char c) { return !std::isalpha(c) || std::isupper(c); })) {
            ret.insert({ iter->second, TERMIN });
        }
        else {
            ret.insert({ iter->second, NOTTER });
        }
    }
    return ret;
}
const std::map<SNL_TOKEN_TYPE, bool> Token_Terminal_Map = make_terminal_map<std::string, SNL_TOKEN_TYPE>(Token_Name_Type_Map);


std::string get_Token_Str(SNL_TOKEN_TYPE tok){
    auto iter = Token_Type_Name_Map.find(tok);
    if( iter == Token_Type_Name_Map.end()){
        return std::string();
    }
    return iter->second;
}

bool is_Token_Terminal(SNL_TOKEN_TYPE tok){
    auto iter = Token_Terminal_Map.find(tok);
    if(iter == Token_Terminal_Map.end()){
        return true;
    }
    return iter->second;
}


