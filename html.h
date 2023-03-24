/*
 * html.h
 *
 *  Created on: 25. 2. 2023
 *      Author: Administrator
 */

#ifndef HTML_H_
#define HTML_H_

void Html_populateEnvVariables(const char*, char*);

const char Html_preamble[] = "HTTP/1.1 200 OK\nContent-Type: text/html\nConnection: close\n\n";
const char Html_head[] = "\
<!DOCTYPE html>\
<html><head>\
<meta http-equiv=\"content-type\" content=\"text/html; charset=ISO-8859-2\">\
    <title>\
        mSniff Dashboard\
    </title>";


const char Html_style[] = "\
    <style>\
    html {\
        font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;\
        color: #e3e3e3;\
    }\
\
    body {\
        background-color: #292929;\
    }\
\
    section {\
        background-color: #4a4a4a;\
        display: grid;\
        justify-content: left;\
        align-content: center;\
        width: 500px;\
        height: 100%;\
        padding: 10px 10px 10px 10px;\
        margin: 10px;\
    }\
\
    h1 {\
        padding: 10px;\
    }\
    form  {\
        display: table;\ 
    }\
    p     {\
        display: table-row;\
    }\
  \
    label { \
        display: table-cell; \
        padding-right: 10px;\
    }\
    \
    input { \
        display: table-cell; \
    }\
   \
    input[type=\"text\"]:disabled {\
        background: #4a4a4a;\
        color: #ffffff;\
        font-size: medium;\
    }\
    \
    input[type=\"text\"] {\
        font-size: medium;\
    }\
\
    input[type=\"submit\"] {\
        font-size: medium;  \
        width: 70px;\ 
    }\
</style>";

const char* Html_script_env_variables = "\
<script>var remote_ip = \"_\"; window.alert(remote_ip);</script>";
//const char Html_body[] = "\
//    </head><body><h1>mSniff Dashboard</h1><section><h2>Network</h2>\
//                <form id=\"form_network\">\
//                    <p><label for=\"dhcp\">DHCP</label><input type=\"radio\" id=\"dhcp\" name=\"network_radio\" value=\"DHCP\" checked=\"checked\"></p><p>\
//                        <label for=\"static\">Static</label>\
//                        <input type=\"radio\" id=\"static\" name=\"network_radio\" value=\"STATIC\"></p><br><p>\
//                        <label for=\"mac_add\">MAC address</label>\
//                        <input type=\"text\" id=\"ip_add\" name=\"network_input_mac\" value=\"00-00-00-11-22-33\" disabled=\"disabled\"></p><p>\
//                        <label for=\"ip_add\"> IP address</label>\
//                        <input type=\"text\" id=\"inet_form\" name=\"mip\" value=\"192.168.1.128\" disabled=\"disabled\">\
//                    </p>\
//                    <p>\
//                        <label for=\"gate_add\">Gateway IP address</label>\
//                        <input type=\"text\" id=\"inet_form\" name=\"gip\" value=\"192.168.1.1\" disabled=\"disabled\">\
//                    </p>\
//                    <p>\
//                        <label for=\"sub_mask\">Subnet mask</label>\
//                        <input type=\"text\" id=\"inet_form\" name=\"msk\" value=\"255.255.255.0\" disabled=\"disabled\">\
//                    </p>\
//                    <br>\
//                    <p>\
//                        <input type=\"submit\" id=\"sub\" value=\"Refresh\">\
//                    </p>\
//                </form>\
//                <script>\
//                    const dhcp_radios = document.querySelectorAll('input[name=\"network_radio\"]');\
//                    for (const radio of dhcp_radios) {\
//                        radio.addEventListener('change', () => {\
//                            document.querySelectorAll('input[id=\"inet_form\"]').forEach(element => {\
//                                element.disabled = document.getElementById(\"dhcp\").checked;\
//                            });\
//                        })\
//                    }\
//                </script>\
//        </section>
 const char Html_remote_target_start[]="<section>\
            <h2>Remote Target</h2>\
            <form>\
                <p>\
                    <label for=\"target_ip\">Target IP address</label>\
                    <input type=\"text\" id=\"target_ip\" value=\"";
const char Html_remote_target_ending[] = "\">\
                </p>\
                <br>\
                <p>\
                    <input type=\"submit\" id=\"target_submit\" value=\"Set\">\
                </p>\
            </form>\
        </section>\"";
//        <section>\
//            <h2>RF</h2>\
//        </section>\
//        <section>\
//            <h2>Stats</h2>\
//        </section>\
//        <section>\
//            <h2>Analysis</h2>\
//        </section>\
//</body></html>";

#endif /* HTML_H_ */

