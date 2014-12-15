#include "stdlib.h"
#include "util.h"
#include "physical.h"

void datalink_send(char* buffer, int len, int sock);
void datalink_recieve(char* buffer, int len, int sock);
