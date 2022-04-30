# Projeto de pesquisa - Programação de Streaming em clusters

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)  ![Python](https://img.shields.io/badge/Python-3-blue.svg?style=flat&logo=python) ![Spark](https://img.shields.io/badge/Apache_Spark-3.2.1-FFFFFF?style=flat&logo=apachespark&logoColor=#E35A16) ![Hadoop](https://img.shields.io/badge/Hadoop-3.2.3-white.svg)

O objetivo deste projeto é permitir que o aluno avance seus conhecimentos sobre arquitetura de clusters de processamento de Streaming e programação de aplicações para consumo e tratamento de eventos, em tempo real. Esses conhecimentos devem ser adquiridos por meio da configuração de um processador de streaming aliado a um servidor de mensageria para classificação dos eventos. Para atendimento a essa atividade, o projeto foi dividido em duas partes, descritas a seguir:

## Parte 1 - Spark Streaming contabilizando palavras de entrada via socket

Consiste uma aplicação que utiliza da API Apache Spark Streaming com 2 threads de trabalho em Single Node Hadoop Deployment e programação usado Python. Essa aplicação consegue ler palavras enviadas a partir de um socket TCP e apresenta o resultado dessa contabilização no console.

### Para rodar

Para rodar a parte 1 vai ser necessario ter instalado o Hadoop 3.2.3, Spark 3.2.1 e o Python 3. Supondo que você tenha siga as seguintes instruções, caso contrario vá para a sessão de Tutorial de Instalação.

Use do Netcat para enviar as palavras.

```
nc -v -lk -p 9999
``` 

Exporte a variavel LABPATH com o caminho do seu diretorio
```
export LABPATH=/home/giovanna/unb/pspd_unb/Lab04
```

Execute os seguintes comandos.
```
$SPARK_HOME/bin/spark-submit --master local[2] $LABPATH/main.py --hostname localhost --port 9999
```
### Tutorial de Instalação  

#### Instale no Ubuntu

```
sudo apt update
sudo apt install openjdk-8-jdk -y
java -version; javac -version
sudo apt install openssh-server openssh-client -y
```

##### Crie um usuário para o ambiete
```
sudo adduser hadoop
su - hadoop
ssh-keygen -t rsa -P '' -f ~/.ssh/id_rsa
cat ~/.ssh/id_rsa.pub >> ~/.ssh/authorized_keys
chmod 0600 ~/.ssh/authorized_keys
ssh localhost
```

#### Hadoop

A biblioteca de software Apache Hadoop é uma estrutura que permite o processamento distribuído de grandes conjuntos de dados em clusters de computadores usando modelos de programação simples. Ele foi projetado para escalar de servidores únicos para milhares de máquinas, cada uma oferecendo computação e armazenamento local. Em vez de depender de hardware para fornecer alta disponibilidade, a própria biblioteca foi projetada para detectar e lidar com falhas na camada de aplicação, fornecendo um serviço altamente disponível em um cluster de computadores, cada um dos quais pode estar propenso a falhas.

##### Instale Hadoop
```
wget https://dlcdn.apache.org/hadoop/common/hadoop-3.2.3/hadoop-3.2.3.tar.gz
tar xzf hadoop-3.2.3.tar.gz
```

##### Configure o arquivo bashrc

Abra o arquivo usando o comando:
```
sudo nano .bashrc
```

Defina as variáveis ​​de ambiente do Hadoop adicionando o seguinte conteúdo ao final do arquivo:
```
#Hadoop Related Options
export HADOOP_HOME=/home/hadoop/hadoop-3.2.3
export HADOOP_INSTALL=$HADOOP_HOME
export HADOOP_MAPRED_HOME=$HADOOP_HOME
export HADOOP_COMMON_HOME=$HADOOP_HOME
export HADOOP_HDFS_HOME=$HADOOP_HOME
export YARN_HOME=$HADOOP_HOME
export HADOOP_COMMON_LIB_NATIVE_DIR=$HADOOP_HOME/lib/native
export PATH=$PATH:$HADOOP_HOME/sbin:$HADOOP_HOME/bin
export HADOOP_OPTS="-Djava.library.path=$HADOOP_HOME/lib/native"
```

Aplique as alterações ao ambiente em execução atual usando o seguinte comando:

```
source ~/.bashrc
```

##### Configure o java 8

Selecione o PATH com os seguintes comandos
```
which javac
readlink -f /usr/bin/javac
```

Abra o arquivo hadoop-env.sh
```
sudo nano $HADOOP_HOME/etc/hadoop/hadoop-env.sh
```

Remova o comentário da variável $JAVA_HOME e adicione o caminho completo.
```
export JAVA_HOME=/usr/lib/jvm/java-8-openjdk-amd64
```

##### Single Node Hadoop Deployment

Abra o arquivo: 
```
sudo nano $HADOOP_HOME/etc/hadoop/core-site.xml
```

Adicione a seguinte configuração para substituir os valores padrão do diretório temporário e adicione sua URL do HDFS para substituir a configuração padrão do sistema de arquivos local:
```
<configuration>
<property>
  <name>hadoop.tmp.dir</name>
  <value>/home/hadoop/tmpdata</value>
</property>
<property>
  <name>fs.default.name</name>
  <value>hdfs://127.0.0.1:9000</value>
</property>
</configuration>
```

Abra o arquivo: 
```
sudo nano $HADOOP_HOME/etc/hadoop/hdfs-site.xml
```

Adicione a seguinte configuração ao arquivo e, se necessário, ajuste os diretórios NameNode e DataNode aos seus locais personalizados:

```
<configuration>
<property>
  <name>dfs.data.dir</name>
  <value>/home/hadoop/dfsdata/namenode</value>
</property>
<property>
  <name>dfs.data.dir</name>
  <value>/home/hadoop/dfsdata/datanode</value>
</property>
<property>
  <name>dfs.replication</name>
  <value>1</value>
</property>
</configuration>
```

Abra o arquivo: 
```
sudo nano $HADOOP_HOME/etc/hadoop/mapred-site.xml
```

Anexe a seguinte configuração ao arquivo:
```
<configuration> 
<property> 
  <name>mapreduce.framework.name</name> 
  <value>yarn</value> 
</property> 
</configuration>
```

Abra o arquivo: 
```
sudo nano $HADOOP_HOME/etc/hadoop/yarn-site.xml
```

Anexe a seguinte configuração ao arquivo:
```
<configuration>
<property>
  <name>yarn.nodemanager.aux-services</name>
  <value>mapreduce_shuffle</value>
</property>
<property>
  <name>yarn.nodemanager.aux-services.mapreduce.shuffle.class</name>
  <value>org.apache.hadoop.mapred.ShuffleHandler</value>
</property>
<property>
  <name>yarn.resourcemanager.hostname</name>
  <value>127.0.0.1</value>
</property>
<property>
  <name>yarn.acl.enable</name>
  <value>0</value>
</property>
<property>
  <name>yarn.nodemanager.env-whitelist</name>   
  <value>JAVA_HOME,HADOOP_COMMON_HOME,HADOOP_HDFS_HOME,HADOOP_CONF_DIR,CLASSPATH_PERPEND_DISTCACHE,HADOOP_YARN_HOME,HADOOP_MAPRED_HOME</value>
</property>
</configuration>
```

##### Formate HDFS NameNode

```
hdfs namenode -format
```

##### Inicie o cluster do Hadoop

```
$HADOOP_HOME/sbin/start-dfs.sh
jps
```

##### Acesse a interface do usuário do Hadoop no navegador

O número de porta padrão 9870 fornece acesso à IU do Hadoop NameNode:
```
http://localhost:9870
```

A porta padrão 9864 é usada para acessar DataNodes individuais diretamente do seu navegador:
```
http://localhost:9864
```

O YARN Resource Manager está acessível na porta 8088:

```
http://localhost:8088
```

#### Apache Spark

Apache Spark é um framework de código fonte aberto para computação distribuída. Foi desenvolvido no AMPLab da Universidade da Califórnia e posteriormente repassado para a Apache Software Foundation que o mantém desde então. Spark provê uma interface para programação de clusters com paralelismo e tolerância a falhas.

```
wget https://dlcdn.apache.org/spark/spark-3.2.1/spark-3.2.1-bin-hadoop3.2.tgz
tar xzf spark-3.2.1-bin-hadoop3.2.tgz
```
##### Configure o arquivo bashrc

Abra o arquivo usando o comando:
```
sudo nano .bashrc
```

Defina as variáveis ​​de ambiente do Spark adicionando o seguinte conteúdo ao final do arquivo:

```
export SPARK_HOME=/home/hadoop/spark-3.2.1-bin-hadoop3.2
export PATH=$PATH:$SPARK_HOME/bin:$SPARK_HOME/sbin
```

Aplique as alterações ao ambiente em execução atual usando o seguinte comando:
```
source ~/.bashrc
```

##### Inicie o Spark

```
start-master.sh
start-worker.sh spark://ubuntu:7077
```

Então siga o tutorial de execução normalmente.

### Tutoriais usados 

- https://phoenixnap.com/kb/install-hadoop-ubuntu
- https://computingforgeeks.com/how-to-install-apache-spark-on-ubuntu-debian/
- https://spark.apache.org/docs/latest/streaming-programming-guide.html