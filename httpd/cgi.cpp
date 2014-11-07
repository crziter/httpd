#include <sstream>
// #include <io.h>
// #include <fcntl.h>
#include "file_operations.h"
#include "cgi.h"
#include "port.h"

using std::cout;
using std::endl;

enum PIPES { READ, WRITE }; /* Constants 0 and 1 for READ and WRITE */

cgi::cgi(host_info& hi, cgi_info& ci, http_request& rq, configuration& conf)
    :_hi(hi), _ci(ci), _req(rq), _conf(conf)
{

}

bool cgi::process(std::string& content)
{
    std::string gate_interface = "CGI/1.1";
    std::string redirect_status = "200";

    set_env(SERVER_NAME, _req.host());
    set_env(GATEWAY_INTERFACE, gate_interface);
    set_env(REQUEST_URI, _req.uri().resource());
    set_env(REQUEST_METHOD, _req.method_str());
    set_env(SERVER_PROTOCOL, _req.version());
    set_env(SCRIPT_FILENAME, _hi.location + file_operations::convert_path(_req.uri().path()));
    set_env(SCRIPT_NAME, _req.uri().path());
    set_env(REDIRECT_STATUS, redirect_status);
    set_env(QUERY_STRING, _req.uri().query());
    set_env(SERVER_SOFTWARE, _conf.server_name());

    if (_req.method() == http_method::POST) {
        std::string clength = std::to_string(_req.content().length());
        set_env(CONTENT_LENGTH_CGI, clength);
    }

    SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES) };
    sa.bInheritHandle = 1;
    sa.lpSecurityDescriptor = NULL;

    HANDLE hStdoutR, hStdoutW, hStdinR, hStdinW;
    CreatePipe(&hStdoutR, &hStdoutW, &sa, 0);
    SetHandleInformation(hStdoutR, HANDLE_FLAG_INHERIT, 0);
    CreatePipe(&hStdinR, &hStdinW, &sa, 0);
    SetHandleInformation(hStdinW, HANDLE_FLAG_INHERIT, 0);

    STARTUPINFOA si = { sizeof(STARTUPINFOA) };
    PROCESS_INFORMATION pi;
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdOutput = hStdoutW;
    si.hStdInput = hStdinR;

    //char env[255] = "REQUEST_METHOD=POST\0CONTENT_LENGTH=17\0CONTENT_TYPE=application/x-www-form-urlencoded\0SCRIPT_FILENAME=D:\\test.php";
    if (!CreateProcessA(NULL, (LPSTR)_ci.cmd.c_str(), NULL, NULL, 1, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi)) {
        std::cout << "ERROR - CGI" << std::endl;
        return false;
    }
    CloseHandle(hStdoutW);
    CloseHandle(hStdinR);


    DWORD dwWritten = 0;
    //Write post data here? 
    if (!WriteFile(hStdinW, _req.content().c_str(), _req.content().length(), &dwWritten, NULL)) {
        std::cout << "ERROR - CGI" << std::endl;
        return false;
    }

    CloseHandle(hStdinW);

    char buf[1000] = { 0 };
    DWORD dwRead = 0;
    while (ReadFile(hStdoutR, buf, sizeof(buf), &dwRead, NULL) && dwRead != 0){
        content.append(buf, dwRead);
    }
    // printf("|\n\nEND");
    CloseHandle(hStdoutR);

//     SECURITY_ATTRIBUTES attr;
//     attr.nLength = sizeof(SECURITY_ATTRIBUTES);
//     attr.bInheritHandle = TRUE;
//     attr.lpSecurityDescriptor = NULL;
// 
//     HANDLE so_r, so_w, si_r, si_w;
//     CreatePipe(&so_r, &so_w, &attr, 0);
//     SetHandleInformation(&so_r, HANDLE_FLAG_INHERIT, 0);
//     CreatePipe(&si_r, &si_w, &attr, 0);
//     SetHandleInformation(&si_w, HANDLE_FLAG_INHERIT, 0);
// 
//     PROCESS_INFORMATION piProcInfo;
//     STARTUPINFOA siStartInfo;
//     ZeroMemory(&piProcInfo, sizeof(piProcInfo));
//     ZeroMemory(&siStartInfo, sizeof(siStartInfo));
// 
//     siStartInfo.cb = sizeof(STARTUPINFOA);
//     siStartInfo.hStdOutput = so_w;
//     siStartInfo.hStdInput = si_r;
//     siStartInfo.dwFlags |= STARTF_USESTDHANDLES;
// 
//     auto bSuccess = CreateProcessA(NULL,
//         (LPSTR)_ci.cmd.c_str(),     // command line 
//         NULL,          // process security attributes 
//         NULL,          // primary thread security attributes 
//         TRUE,          // handles are inherited 
//         0,             // creation flags 
//         NULL,          // use parent's environment 
//         NULL,          // use parent's current directory 
//         &siStartInfo,  // STARTUPINFO pointer 
//         &piProcInfo);  // receives PROCESS_INFORMATION 
// 
// //     CloseHandle(piProcInfo.hProcess);
// //     CloseHandle(piProcInfo.hThread);
// 
//     DWORD dwWritten;
//     WriteFile(si_w, _req.content().c_str(), _req.content().length(), &dwWritten, NULL);
//     CloseHandle(si_w);
// 
//     char buffer[128];
//     DWORD dwRead;
//     for (;;) {
//         auto rs = ReadFile(so_r, buffer, sizeof(buffer), &dwRead, NULL);
//         if (!rs || dwRead == 0) break;
// 
//         content.append(buffer, dwRead);
//     }

//     TerminateProcess(piProcInfo.hProcess, 0);
//     CloseHandle(si_r);
//     CloseHandle(si_w);
//     CloseHandle(so_r);
//     CloseHandle(so_w);

    //IF_WINDOWS({
//         FILE *fp = _popen(_ci.cmd.c_str(), "wr");
//         if (fp) {
// 
//             if (_req.method() == http_method::POST) {
//                 int wten = fwrite(_req.content().c_str(), 1, _req.content().length(), fp);
//                 IF_DEBUG({
//                     std::cout << "Written " << wten << " bytes to CGI" << std::endl;
//                 });
//             }
// 
//             char buffer[128];
//             int rd = 0;
//             do {
//                 rd = fread(buffer, 1, sizeof(buffer), fp);
// 
//                 if (rd <= 0) break;
//                 content.append(buffer, rd);
//             } while (true);
// 
//             if (feof(fp)) {
//                 fclose(fp);
//             } else {
//                 return false;
//             }
// 
//             return true;
//         } else return false;
    //});
    
    return true;
}

void cgi::set_env(const std::string& name, std::string& value)
{
#if defined(_WIN32) || defined(WIN32)
    std::stringstream ss;
    ss << name << "=" << value;
    _putenv(ss.str().c_str());
#else
    setenv(name.c_str(), value.c_str(), 1);
#endif
}
