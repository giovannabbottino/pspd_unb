# Copyright 2007 - 2021, Alan Antonuk and the rabbitmq-c contributors.
# SPDX-License-Identifier: mit

add_library(utils OBJECT)
target_sources(utils PRIVATE
  utils.h
  utils.c)

target_link_libraries(utils PRIVATE rabbitmq::rabbitmq)

add_executable(consumer consumer.c)
target_link_libraries(consumer PRIVATE utils rabbitmq::rabbitmq)

add_executable(producer producer.c)
target_link_libraries(producer PRIVATE utils -lm rabbitmq::rabbitmq)