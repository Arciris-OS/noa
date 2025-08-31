#include <openssl/sha.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


char *get_noa_expected_hash() {

}


int check_noa(const char* sha256) {
	char *expected = get_noa_expected_hash();
}
