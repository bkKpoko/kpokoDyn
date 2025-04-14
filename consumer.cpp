#include "consumer.h"
#include <cstddef>
#include <cstdio>
#include <fcntl.h>
#include <string>
#include <sys/mman.h>

Consumer::Consumer(std::string shmName){
  init(shmName);
  int fd = shm_open(shmName.c_str(), O_RDWR, 0666);
  size_t shm_size = sizeof(SharedBuffer) + sizeof(double) * array_size; 
  sbuf = (SharedBuffer*)mmap(NULL, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
}

Consumer::~Consumer(){
  munmap(sbuf, sizeof(SharedBuffer));
}

void Consumer::update(){
  sem_wait(&sbuf->sem_consume);
  for (size_t i = 0; i < sbuf->data_size; ++i) {
    data[i] = sbuf->data[i];
    // printf("data[%zi] = %g\n", i, data[i]);
  }
  sem_post(&sbuf->sem_produce);
}

void Consumer::init(std::string shmName){
  int fd = shm_open(shmName.c_str(), O_RDWR, 0666);
  size_t size = sizeof(SharedBuffer);
  sbuf = (SharedBuffer*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  array_size = sbuf->data_size;
  data.resize(array_size);
}
