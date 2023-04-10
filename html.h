/*
 * html.h
 *
 *  Created on: 25. 2. 2023
 *      Author: Administrator
 */

#ifndef HTML_H_
#define HTML_H_


char* Html_sectionRemoteTarget(const char*);
char* Html_sectionNetworkInfo(const char*, const char*, const char*, const char*);
const char* Html_preamble();
const char* Html_head();
const char* Html_style();
const char* Html_script();







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

