#include "IL_GetMeasurement_linux.h"

#define KEYENCE "192.168.0.30"
#define KEYENCE_PORT 64000
void PrintHexa(char* buff, size_t len);

void GetMeasurement() {
   //SOCKET sock; WSADATA wsa;
   struct sockaddr_in sock_addr;
   int ret = 0;
   int sockfd;
   //WSAStartup(MAKEWORD(2, 2), &wsa);
   sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
   //memset(&sock_addr, 0, sizeof(sock_addr));


   sock_addr.sin_family = AF_INET;
   sock_addr.sin_addr.s_addr = inet_addr(KEYENCE);
   sock_addr.sin_port = htons(KEYENCE_PORT);

   char IL_GetMeasurementValue[] = { 0x4D, 0x30, 0x0D, 0x0A }; // register, register, CR, LF
   size_t IL_GetMeasurementValue_s = sizeof(IL_GetMeasurementValue);
   char OutputMeasurement[15];
   cout << " running?" << endl;
   if (connect(sockfd, (struct sockaddr*)&sock_addr, sizeof(sock_addr)) == -1) {
      perror("connect()");
      close(sockfd);
      return;
   }
   cout << "here?" << endl;
   while (1) {
       cout << " before sending?" << endl;
      if (send(sockfd, IL_GetMeasurementValue, IL_GetMeasurementValue_s, 0) == -1) {
         perror("send()");
         close(sockfd);
         return;
      }
      ret = recv(sockfd, OutputMeasurement, 15, 0);
      if (ret == -1) {
         perror("recv()"); close(sockfd);
         return;
      }
      //PrintHexa(OutputMeasurement, ret);
      int msb = 0, data = 0;
      double OutputMeasurement_mm = 0;
      msb = OutputMeasurement[3];
      data = OutputMeasurement[8] - '0';
      for (int i = 0; i < 5; i++) {
         data = OutputMeasurement[8 + i] - '0';
         OutputMeasurement_mm += data * 10 * pow(0.1, i);
      }
      cout << "Output mm : " << OutputMeasurement_mm << endl;
   }
   close(sockfd);
   system("pause");
}

void PrintHexa(char* buff, size_t len) {
   size_t i;
   for (i = 0; i < len; i++)
   {
      printf("%02X ", (unsigned char)buff[i]);
   }
   printf("\n");
}