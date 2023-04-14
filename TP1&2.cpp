#include <QCoreApplication>
#include <QtMqtt/QMqttClient>
#include <QDebug> 

int main(int argc_var, char *argv_t[])
{
    // Creating an instance of Qt Core
    QCoreApplication A(argc_var, argv_t); 
    
    // Creating a client
    QMqttClient Client; 

    // Setting the broker host
    Client.setHostname("broker.emqx.io"); 
    // Setting the broker port
    Client.setPort(1883); 

    QObject::connect(&Client, &QMqttClient::connected, [&](void) {
        qDebug() << "Connected to MQTT broker.";

        QString s_topic = "/ynov/bordeaux/";
        qint32 qos_var = 0;
        
        // Subscribing to a topic
        if (!Client.subscribe(s_topic, qos_var)) { 
            qDebug() << "Error while subscribing to :" << s_topic;
            return 1;
        } else {
            qDebug() << "Subscribed to topic:" << s_topic;
        }

        QString s_message = "Message Leo";
        
        // Publishing a message
        if (!Client.publish(s_topic, s_message.toUtf8(), qos_var)) { 
            qDebug() << "Published message:" << s_message << "to topic:" << s_topic;
            return 1;
        } else {
            qDebug() << "Error while publishing :" << s_message << "to topic:" << s_topic;
        }
    });

    QObject::connect(&Client, &QMqttClient::messageReceived, [&](const QByteArray &message, const QMqttTopicName &topic) {
        qDebug() << "Received message:" << message << "from topic:" << topic.name();
    });

    // Connecting to the MQTT broker
    Client.connectToHost(); 

    // Starting the Qt Core event loop
    return A.exec(); 
}
