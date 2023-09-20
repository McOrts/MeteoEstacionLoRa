# Estacioón meteorológica LoRaWAN
Estación metereológica DIY con comunicaciones LoRaWAN utilizando la red TTN Community.

### Montaje

<img src="img/MeteoEstacionLoRa_bb.png" align="center" />

### Configuración IDE Arduino
Lo para poder programar adecuadamente el dispositivo tendremos que configurar el entorno de programación de Arduino con las configuraciones de hardware y librerías de esta placa.
1. Añadir la URL https://github.com/HelTecAutomation/CubeCell-Arduino/releases/download/V1.5.0/package_CubeCell_index.json a Preferences->Settings->Additional boards:
2. Añadir la librería de CayenneLPP. **Importante: versión 1.1.0**:

<img src="./img/lib_cayenne.png"  align="center" />

3. Si aparece un aviso de instalar otras librerias con dependencias. Seleccionar opción de instalar todas:

<img src="./img/library_dependencies.png"  align="center" />

4. Añadir la librería del sesnor de temperatura y humedad: DHT sensor library de Adafruit.
<img src="./img/DHT22_lib_arduino.png"  align="center" />

6. Seleccionar la tarjeta CubeCell HTCC-AB01 y configurar todos sus parámetros así:

<img src="./img/arduino_board_setup_cubecell.png"  align="center" />

### Subir el sketch de Arduino
Con el cable conectado seleccionamos el puerto USB que nuestro ordenador haya asignado. Una vez compilado y cargado en el dispositivo. Abrimos el monitor serie para comprobar que log de actividad del programa es correcto.

### Configuración del sensor en la nube (TTN)

Vamos a utilizar los servicios de The Things Network que es un servicio en la nube al que están conectados los _gateway_ libres de la las Comunidades TTN. 
La ruta de la información es la siguiente:
1. El sensor (_nodo_) transmite por radio con modulación LoRa el trama de datos que recibe el _gateway_
2. Este _gateway_ está conectado a internet para retransmitir la trama, hora como paquete de datos.
3. Los servidores de TTN decodifican el mensaje y ofrecen diferentes integraciones y _endpoints_ para que nuestras aplicaciones utilicen la información leída por los sensores.

Hay muchas variantes para implementar este enrutamiento. Para este se ha elegido estas configuraciones:
- Dispositivo es del tipo OTAA (Over-The-Air Activation) un dispositivo final realiza un procedimiento de identificación con una red LoRaWAN, durante el cual se asigna una _DevAddr_ dinámica a un dispositivo final y las claves raíz se utilizan para derivar claves de sesión. Por lo tanto, _DevAddr_ y las claves de sesión cambian a medida que se establece cada nueva sesión.
- Publicación del paquete de-codificado en una cola MQTT a la que nuestra aplicación está subscrita.

<img src="./img/TTN_p1.jpg" width="200" align="left" />

Estos son los pasos a seguir empezando por acceder a la aplicación _back-end_ de TTN en esta URL: https://console.cloud.thethings.network/ en la que seleccionaremos nuestra región y luego nos loguearemos con nuestro usuario registrado.

<br>
<br>
<br>
<br>
<br>
<br>
<br>

### Registro de la aplicación
En TTN los dispositivos (_devices_) iguales, se agrupan en una aplicación desde la cual serán registrados. por lo tanto, primero hay que añadir una aplicación:

En el formulario de alta de aplicación rellenaremos estos campos: 
- Para el _Application ID_, elige un identificador único, en minúsculas, puedes usar caracteres alfanuméricos peor no guiones ´-´ consecutivos.
- Para  _Description_, elige la descripción que prefieras.
- No hace falta poner nada en _Application EUI_ 
- Presiona _Create application_ para finalizar

<img src="./img/TTN_p2.jpg" width="250" align="left" />
<img src="./img/TTN_p3.jpg" width="250" align="left" />
<img src="./img/TTN_p4.jpg" width="300" align="right" />

<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>

Aparecerá ahora la página con la nueva aplicación añadida donde encontrarás un botón de _+ Add end device_ necesario para crear los dispositivos asociados. 

<img src="./img/TTN_app_created.png" align="center" />

### Registro del dispositivo
En TTN un dispositivo (devide) representa la configuración de lo que también llama nodo (node) que a fin de cuentas es nuestro sensor. 
Al acceder al formulario de registro nos quedamos en la pestaña de _From The LoRaWAN Device Repository_ ya que estamos usando un microcontrolador conocido.
Ahora tenemos que seleccionar el tipo, modelo y marca de nuestro sensor tal y como aparece en la captura inferior. 
<img src="./img/TTN_device.png"  align="center" />

El la casilla inferior vemos que pide una clave de JoinEUI que normalmente nos la daría el fabricante. Pero en nuestro caso utilizaremos una sencilla aplicación web para generarla:  https://descartes.co.uk/CreateEUIKey.html
<img src="./img/Create_random_EUI.png"  align="center" />

Lo siguiente será generar las _DevEUI_ y _AppKey_ pulsando los correspondientes iconos de la derecha para que se genere automáticamente.
Finalmente queda rellenar el _Device ID_ que será el nombre único de este nodo.

<img src="./img/TTN_device_keys.png"  align="center" />

Finalmente pulsaremos _Register en device_ para que, seguidamente aparezca la pantalla con la configuración de nuestro nuevo nodo.Aquí podremos ver los datos de configuración y parámetros que necesitamos por ser un dispositivo de tipo ABP. Y que tendremos que pasar al fichero de configuración settings.h que se cargará en el _sketch_ del IDE de Arduino.

<img src="./img/TTN_device_created.png"  align="center" />

Para poder copiar las _keys_ en el formato "array" que se requiere en el fichero de configuración: "settings.h" 
Nota: hay que pulsar el icono <> de "_toggle array formating_" para tener el formato adecuado para este fichero.

```cpp
// LoRaWAN Configuration

/* OTAA para*/
static uint8_t devEui[] = {  };
static uint8_t appEui[] = {  };
static uint8_t appKey[] = {  };

uint16_t userChannelsMask[6]={ 0x00FF,0x0000,0x0000,0x0000,0x0000,0x0000 };
static DeviceClass_t lorawanClass = LORAWAN_CLASS;

// Devise location
const float latitude = 39.573;
const float longitude = 2.732;
const int alt = 20;

// Other params
float SensorId= 10.01;// Sensor  identifcator number 
const int DutyCycle = 600000; // Transmision and reading period
const int Sleep4NoNoise = 300000; // Sleep period when low noise 
const int LowNoiseLevel = 180; // Limit for low noise mode
```

### Formato de la trama

Desde el menú vertical izquierdo accederemos al _Payload Formatters_ y la opción Uplink para definir cómo decodificar la trama de datos de nuestro mensaje LoRa que hemos establecido en el firmware. En nuestro caso hemos usado la librería CayenneLPP y esto es lo que aquí tenemos de seleccionar como tipo:

<img src="./img/TTN_app_uplink_format.png" width="400" align="center" />

De esta manera el mensaje MQTT que vamos a utilizar para leer los valores del sensor; será generado por backend de TTN en una estructura JSON perfectamente formateada:

```json
{
 "uplink_message":{
      "f_port":2,
      "f_cnt":539,
      "frm_payload":"AQID6QFlAE8CZQDDAogGCGAAaiwAC7g=",
      "decoded_payload":{
         "analog_in_1":10.02,
         "gps_2":{
            "altitude":30,
            "latitude":39.936,
            "longitude":3.718
         },
         "luminosity_1":79,
         "luminosity_2":195
      }
}
```
### Integración MQTT
Por último necesitaremos las credenciales de acceso al broker MQTT que TTN nos ofrece como una opción de integración para poder suscribirnos a topic desde nuestra aplicación:

<img src="./img/TTN_app_mqtt.png" width="400" align="center" />

**IMPORTANTE**: copia el password ahora porque una vez cerrada esta ventana. No tendrás opción a consultarlo.

## Agradecimientos
- **Joaquin González Torres**. Compañero Técnico de Telecomunicaciones
- **Joan Carbonell**. Profesor del IES Politecnic

<img src="img/anemometer_deployed.GIF"  align="center" />
