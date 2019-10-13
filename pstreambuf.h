#include <iostream>
#include <streambuf>
#include <memory>
#include <array>
#include <functional>
#include <string>
#include <stdio.h>
#include <string.h>

class pstreambuf : public std::streambuf {
  public:
    pstreambuf(std::string cmd) : 
                              m_cmd{cmd}, pp {popen(m_cmd.c_str(), "r"), fp} {
      setg(buffer.begin(), buffer.begin(), buffer.begin());
      char *check = fgets(buffer.data(), 1024, pp.get());
      if(check != nullptr) {
        /*We either got a whole line with \n\0 at the end of the buffer
          or we got a "string" with a \0 at the end
        */
        setg(buffer.begin(), buffer.begin(), buffer.begin()+strlen(check));
      } else {
        std::cerr << "something went wrong\n";
       /* nullptr on error or if EOF without receiving data */ 
      }
    }

  private:
    std::string m_cmd;
    const std::function<void(FILE*)> fp = [](FILE *f) { pclose(f); };
    std::unique_ptr<FILE, decltype(fp)> pp;
    std::array<char, 1024> buffer;

  protected:
    virtual int underflow() {
      if(gptr() < egptr()) {
        return traits_type::to_int_type(*gptr());
      }

      char *check = fgets(buffer.data(), 1024, pp.get());
      if(check != nullptr) {
        /*We either got a whole line with \n\0 at the end of the buffer
          or we got a "string" with a \0 at the end
        */
        setg(buffer.begin(), buffer.begin(), buffer.begin()+strlen(check));
        return traits_type::to_int_type(*gptr());
      } else {
        /* nullptr on error or if EOF without receiving data */ 
        return traits_type::eof();
      }
    }

    virtual std::streamsize in_avail() {
      std::string s { buffer.data() };
      return static_cast<std::streamsize>(s.size());
    }
};

