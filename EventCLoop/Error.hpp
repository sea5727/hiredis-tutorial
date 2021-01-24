#pragma once



namespace EventCLoop
{
    class Error {
        std::string msg;
        bool flag = false;
    public:
        Error() = default;
        Error(const std::string & what)
            : msg{what}
            , flag{true} {}
        
        void
        clear(){
            flag = false;
            msg = "";
        }
        operator bool() const{
            return flag;
        }
        const char * what() const noexcept{
            return msg.c_str();
        }
    };
}