#ifndef CONSUMER
#define CONSUMER

#include <cstddef>
#include <semaphore.h>
#include <string>
#include <vector>


class Consumer {
public:
  Consumer(std::string shmName);
  ~Consumer();
  void update();

  typedef struct {
    sem_t sem_produce;     
    sem_t sem_consume;     
    size_t data_size;
    double data[];
  } SharedBuffer;


public:
  SharedBuffer* sbuf;
  size_t array_size;
  std::vector<double> data;


private:
  void init(std::string shmName);
};

#endif // !CONSUMER
