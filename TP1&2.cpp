#include <QCoreApplication>
#include <QtMqtt/QMqttClient>
#include <QDebug> 

int main(int argc_var, char *argv_t[])
{
    QCoreApplication A(argc_var, argv_t); // Creating an instance of Qt Core application

    QMqttClient Client; // Creating an MQTT client

    Client.setHostname("broker.emqx.io"); // Setting the MQTT broker host
    Client.setPort(1883); // Setting the MQTT broker port

    QObject::connect(&Client, &QMqttClient::connected, [&](void) {
        qDebug() << "Connected to MQTT broker.";

        QString s_topic = "/ynov/bordeaux/";
        qint32 qos_var = 0;
        if (!Client.subscribe(s_topic, qos_var)) { // Subscribing to an MQTT topic
            qDebug() << "Error subscribing to topic:" << s_topic;
            return 1;
        } else {
            qDebug() << "Subscribed to topic:" << s_topic;
        }

        QString s_message = "Message Leo";
        if (!Client.publish(s_topic, s_message.toUtf8(), qos_var)) { // Publishing an MQTT message
            qDebug() << "Published message:" << s_message << "to topic:" << s_topic;
            return 1;
        } else {
            qDebug() << "Error publishing message:" << s_message << "to topic:" << s_topic;
        }
    });

    QObject::connect(&Client, &QMqttClient::messageReceived, [&](const QByteArray &message, const QMqttTopicName &topic) {
        qDebug() << "Received message:" << message << "from topic:" << topic.name();
    });

    Client.connectToHost(); // Connecting to the MQTT broker

    return A.exec(); // Starting the Qt Core event loop
}
