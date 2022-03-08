# Lab03 – Construindo aplicações distribuídas usando o paradigma publish-subscriber

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)  ![C](https://img.shields.io/badge/Solutions-blue.svg?style=flat&logo=c)

## Para rodar 

```
git clone https://github.com/alanxz/rabbitmq-c.git 
cp src/ -r rabbitmq-c/
cp CMakeLists.txt rabbitmq-c/CMakeLists.txt 
cd rabbitmq-c 
mkdir build && cd build 
cmake .. 
cmake --build . 
cd src/ 
sudo docker run -it --rm --name rabbitmq -p 5672:5672 -p 15672:15672 rabbitmq:3.9-management
./consumer 127.0.0.1 5672 amq.direct lab3 username password  
./producer 127.0.0.1 5672 amq.direct lab3 username password
```