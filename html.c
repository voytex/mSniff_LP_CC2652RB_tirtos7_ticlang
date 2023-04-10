/*
 * html.c
 *
 *  Created on: 7. 4. 2023
 *      Author: vojtechlukas
 */


#include <stdio.h>
#include <string.h>
#include "html.h"

const char preamble[] = "HTTP/1.1 200 OK\nContent-Type: text/html\nConnection: close\n\n";
const char head[] = "<!DOCTYPE html><html><head><meta http-equiv=\"content-type\" content=\"text/html; charset=ISO-8859-2\"><title>mSniff Dashboard</title>";
const char style[] = "<style>"\
    "html {font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;color: #e3e3e3;}"\
    "body {background-color: #292929;}"\
    "section {background-color: #4a4a4a;display: grid;justify-content: left;align-content: center;"\
             "width: 500px;height: 100%;padding: 10px 10px 10px 10px;margin: 10px;}"\
    "h1 {padding: 10px;}"\
    "form {display: table;}"\
    "p {display: table-row;}"\
    "label { display: table-cell;padding-right: 10px;}"\
    "input { display: table-cell;}"\
    "input[type=\"text\"]:disabled {background: #4a4a4a;color: #ffffff;font-size: medium;}"\
    "input[type=\"text\"] {font-size: medium;}"\
    "input[type=\"submit\"] {font-size: medium;width: 70px;}</style><body><h1>mSniff Dashboard</h1>";

/*
 * private
 */
const char networkInfo[]="<section> <h2>Network</h2> <form id=\"form_network\"><p> <label for=\"dhcp\">DHCP</label>"
                          "<input type=\"radio\" id=\"dhcp\" name=\"dhcp\" value=\"1\" ></p><p> <label for=\"static\">Static</label>"
                          "<input type=\"radio\" id=\"static\" name=\"dhcp\" value=\"0\" checked></p><br><p> <label for=\"mac_add\">MAC address</label> "
                          "<input type=\"text\" id=\"mac\" name=\"mac\" value=\"%s\" disabled></p><p> <label for=\"ip_add\"> IP address</label> "
                          "<input type=\"text\" id=\"ip\" name=\"tip\" tag=\"sw_dhcp\" value=\"%s\" ></p><p> <label for=\"gate_add\">Gateway IP address</label> "
                          "<input type=\"text\" id=\"ip\" name=\"gip\" tag=\"sw_dhcp\" value=\"%s\" ></p><p> <label for=\"sub_mask\">Subnet mask</label> "
                          "<input type=\"text\" id=\"ip\" name=\"msk\" tag=\"sw_dhcp\" value=\"%s\" ></p><br><p> "
                          "<input type=\"submit\" id=\"sub\" value=\"Refresh\"></p> </form></section>";

const char remoteTarget[]="<section> <h2>Remote Target</h2> <form action=\"/\" method=\"\" get><p> <label for=\"target_ip\">Target IP address</label> "
                         "<input type=\"text\" id=\"ip\" name=\"target_ip\" value=\"%s\"></p><br><p> "
                         "<input type=\"submit\" value=\"Set\"></p> </form></section>";

const char script[]     ="</body><script>const ip_regex = \"^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$\";"
                         "document.querySelectorAll('input[id=\"ip\"]').forEach(element => { console.log(element); element.setAttribute(\"pattern\", ip_regex);});"
                         "const dhcp_radios = document.querySelectorAll('input[name=\"dhcp\"]');"
                         "for (const radio of dhcp_radios) { radio.addEventListener('change', () => "
                         "{document.querySelectorAll('input[tag=\"sw_dhcp\"]').forEach(element => { element.disabled = document.getElementById(\"dhcp\").checked;}); })}"
                         "</script></html>";

//const char networkInfo[]="<section><h2>Network</h2><form id=\"form_network\"><p><label for=\"dhcp\">DHCP</label>"\
//                         "<input type=\"radio\" id=\"dhcp\" name=\"network_radio\" value=\"DHCP\" checked=\"checked\"></p><p>"\
//                         "<label for=\"static\">Static</label>"\
//                         "<input type=\"radio\" id=\"static\" name=\"network_radio\" value=\"STATIC\"></p><br><p>"\
//                         "<label for=\"mac_add\">MAC address</label>"\
//                         "<input type=\"text\" id=\"ip_add\" name=\"network_input_mac\" value=\"%s\" disabled=\"disabled\"></p><p>"\
//                         "<label for=\"ip_add\"> IP address</label>"\
//                         "<input type=\"text\" id=\"inet_form\" name=\"mip\" value=\"%s\" disabled=\"disabled\">"\
//                         "</p><p><label for=\"gate_add\">Gateway IP address</label>"\
//                         "<input type=\"text\" id=\"inet_form\" name=\"gip\" value=\"%s\" disabled=\"disabled\">"\
//                         "</p><p><label for=\"sub_mask\">Subnet mask</label>"\
//                         "<input type=\"text\" id=\"inet_form\" name=\"msk\" value=\"%s\" disabled=\"disabled\">"\
//                         "</p><br><p><input type=\"submit\" id=\"sub\" value=\"Refresh\">"\
//                         "</p></form><script>"\
//                            "const dhcp_radios = document.querySelectorAll('input[name=\"network_radio\"]');"\
//                            "for (const radio of dhcp_radios) {"\
//                                "radio.addEventListener('change', () => {"\
//                                    "document.querySelectorAll('input[id=\"inet_form\"]').forEach(element => {"\
//                                        "element.disabled = document.getElementById(\"dhcp\").checked;"\
//                                    "});})}</script></section>";

char formatBuffer[2048];

/*
 * Return a pointer to Preamble part of web dashboard
 * 200 OK etc...
 */
const char* Html_preamble()
{
    return preamble;
}

/*
 * Return a pointer to HTML head
 */
const char* Html_head()
{
    return head;
}


/*
 * Returns a pointer to HTML style
 */
const char* Html_style()
{
    return style;
}

/*
 * Returns pointer to script portion of dashboard
 */
const char* Html_script()
{
    return script;
}


/*
 *
 */
char * Html_sectionNetworkInfo(const char* thisMac, const char* thisIp, const char* gateway, const char* mask)
{
    memset(formatBuffer, 0, sizeof(formatBuffer));
    snprintf(formatBuffer, sizeof(formatBuffer), networkInfo, thisMac, thisIp, gateway, mask);
    char* ret = formatBuffer;
    return ret;
}


/*
 *
 */
char* Html_sectionRemoteTarget(const char* targetIp)
{
    memset(formatBuffer, 0, sizeof(formatBuffer));
    snprintf(formatBuffer, sizeof(formatBuffer), remoteTarget, targetIp);
    char* ret = formatBuffer;
    return ret;
}



