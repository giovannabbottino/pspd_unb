# Projeto de pesquisa - Programação de Streaming em clusters

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)  ![Python](https://img.shields.io/badge/Python-3-blue.svg?style=flat&logo=python) ![Spark](https://img.shields.io/badge/Apache_Spark-3.2.1-E35A16?style=flat&logo=apachespark&logoColor=#E35A16) ![Kafka](https://img.shields.io/badge/Apache_Kafka-3.1.0-FFFFFF?style=flat&logo=apachekafka&logoColor=#FFFFFF)

O objetivo deste projeto é permitir que o aluno avance seus conhecimentos sobre arquitetura de clusters de processamento de Streaming e programação de aplicações para consumo e tratamento de eventos, em tempo real. Esses conhecimentos devem ser adquiridos por meio da configuração de um processador de streaming aliado a um servidor de mensageria para classificação dos eventos. Para atendimento a essa atividade, o projeto foi dividido em duas partes, descritas a seguir:

## Parte 1 - Spark Streaming contabilizando palavras de entrada via socket

Consiste uma aplicação que utiliza da API Apache Spark Streaming com tantos threads de trabalho quantos núcleos lógicos em sua máquina e programação usado Python. Essa aplicação consegue ler palavras enviadas a partir de um socket TCP e apresenta o resultado dessa contabilização no console.

### Para rodar

Para rodar a parte 1 vai ser necessario ter instalado o Spark 3.2.1 ou maior. Supondo que você tenha siga as seguintes instruções, caso contrario instale usando o seguinte [tutorial](https://computingforgeeks.com/how-to-install-apache-spark-on-ubuntu-debian/).

Use do socket do Netcat para enviar as palavras.

```
nc -v -lk -p 9999
``` 

Em outro terminal
```
nc -v -lk -p 8888
``` 

Exporte a variavel LABPATH com o caminho do seu diretorio
```
export LABPATH=/mnt/d/repositorios/unb/pspd_unb/Lab04
```

Execute os seguintes comandos.
Usando --master local[*] para executar o Spark localmente com tantos threads de trabalho quantos núcleos lógicos em sua máquina.

As portas e hosts podem ser alterado.
```
$SPARK_HOME/bin/spark-submit --master local[*] $LABPATH/word-count.py --hosts localhost:9999,localhost:8888
```

## Parte 2 - Spark Streaming contabilizando palavras via Apache Kafka

Consiste de um projeto infraestrutura de servidor, frameworks Apache Spark e Apache Kafka e scripts em Python de contabilização. 

Essa aplicação consegue: 
- Número total de palavras recebidas, considerando o intervalo de tempo que a aplicação se manteve ativa.
- Número total de ocorrências de cada palavra, durante o tempo de coleta e processamento da streaming de entrada.
- Número de ocorrências de palavras iniciadas por cada uma das seguintes letras: S, P e R, a cada intervalo de tempo.
- Número de ocorrências de palavras contendo cada uma das seguintes quantidades de caracteres: 6, 8 e 11 a cada intervalo de tempo. 

### Para rodar

Para rodar a parte 2 vai ser necessario ter instalado o Kafta 3.1.0 ou maior. Supondo que você tenha siga as seguintes instruções, caso contrario instale usando o seguinte [tutorial](https://kafka.apache.org/quickstart).

Execute os seguintes comandos.

```
$SPARK_HOME/bin/spark-submit /
--packages org.apache.spark:spark-sql-kafka-0-10_2.12:3.2.1 /
$LABPATH/kafka-word-count.py /
--hosts localhost:9092 /
--topics word-count-events
```

## Tutorial de Instalação  

### Instale no Ubuntu

```
sudo apt update
sudo apt install openjdk-8-jdk -y
java -version; javac -version
sudo apt install openssh-server openssh-client -y
```

### Crie um usuário para o ambiete
```
sudo adduser pspd
su - pspd
ssh-keygen -t rsa -P '' -f ~/.ssh/id_rsa
cat ~/.ssh/id_rsa.pub >> ~/.ssh/authorized_keys
chmod 0600 ~/.ssh/authorized_keys
ssh localhost
```

### Apache Spark

Apache Spark é um framework de código fonte aberto para computação distribuída. Foi desenvolvido no AMPLab da Universidade da Califórnia e posteriormente repassado para a Apache Software Foundation que o mantém desde então. Spark provê uma interface para programação de clusters com paralelismo e tolerância a falhas.

```
wget https://dlcdn.apache.org/spark/spark-3.2.1/spark-3.2.1-bin-hadoop3.2.tgz
tar xzf spark-3.2.1-bin-hadoop3.2.tgz
```
#### Configure o arquivo bashrc

Abra o arquivo usando o comando:
```
sudo nano .bashrc
```

Defina as variáveis ​​de ambiente do Spark adicionando o seguinte conteúdo ao final do arquivo:

```
export SPARK_HOME=/home/pspd/spark-3.2.1-bin-hadoop3.2
export PATH=$PATH:$SPARK_HOME/bin:$SPARK_HOME/sbin
```

Aplique as alterações ao ambiente em execução atual usando o seguinte comando:
```
source ~/.bashrc
```

#### Inicie o Spark

```
start-master.sh
start-worker.sh spark://ubuntu:7077
```

Então siga o tutorial de execução normalmente.


### Apache Kafka

Apache Kafka é uma plataforma open-source de processamento de streams desenvolvida pela Apache Software Foundation, escrita em Scala e Java. O projeto tem como objetivo fornecer uma plataforma unificada, de alta capacidade e baixa latência para tratamento de dados em tempo real.

```
wget https://dlcdn.apache.org/kafka/3.1.0/kafka_2.13-3.1.0.tgz
tar xzf kafka_2.13-3.1.0.tgz
cd kafka_2.13-3.1.0
```

#### Rode o Kafka

Execute os seguintes comandos em terminais diferenets para iniciar todos os serviços na ordem correta:
```
bin/zookeeper-server-start.sh config/zookeeper.properties
```
```
bin/kafka-server-start.sh config/server.properties
```

#### Preparar o tópico de entrada e iniciar o produtor Kafka

Em seguida, criamos o tópico:

```
bin/kafka-topics.sh --create \
    --bootstrap-server localhost:9092 \
    --topic word-count-events
```

e execute o produtor para o envio de mensagens

```
bin/kafka-console-producer.sh  \
--topic word-count-events  \
--bootstrap-server localhost:9092
```
Então siga o tutorial de execução normalmente.
