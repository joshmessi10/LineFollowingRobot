# 🤖 Robot Seguidor de Línea

Este repositorio documenta el desarrollo de un robot seguidor de línea basado en un microcontrolador STM32. 

El proyecto integra control embebido, sensado analógico y una interfaz gráfica auxiliar para la calibración y monitoreo de sensores.

---

## 🎯 Objetivo del Proyecto

Implementar un sistema autónomo capaz de seguir una línea negra sobre fondo blanco utilizando sensores reflectivos y un algoritmo de control embebido. El enfoque incluye una etapa de calibración asistida mediante una interfaz de escritorio desarrollada con Qt, que permite verificar el funcionamiento del sistema de sensado y la estabilidad del voltaje de alimentación.

---

## ⚙️ Componentes del Sistema

### 🧩 Firmware STM32 (Embebido)
- **Lectura de sensores reflectivos** (ADC)
- **Calibración de máximos y mínimos** para cada sensor
- **Cálculo del error de línea y lógica de control**
- **Control de motores vía PWM**
- **Protocolo de comunicación serial estructurado**
- **Transmisión periódica de datos a la PC**

### 🖥️ Aplicación Qt (Visualización)
- Detección y conexión automática a puertos seriales
- Lectura en tiempo real de valores de sensores
- Visualización de valores máximos y mínimos detectados
- Indicadores gráficos de intensidad de señal
- Monitoreo de voltaje del sistema



## 🔄 Protocolo de Comunicación

La comunicación entre el microcontrolador y la PC se realiza mediante una estructura de paquetes con delimitadores, tamaño, datos y verificación por checksum.

Estructura del paquete (bytes):

```
[INICIO][TAMAÑO][DATOS][CHECKSUM][FIN]
```

Tipos de datos enviados:
- Calibración de sensores
- Lecturas en tiempo real
- Nivel de voltaje del sistema
- Tipo de mensaje identificable en campo de control

---

## 📈 Aplicaciones y Escenarios

- Educación en robótica móvil y sistemas embebidos
- Prototipos de vehículos autónomos básicos
- Pruebas de algoritmos de control (P, PD, PID)

## 📎 Requisitos

- Placa con microcontrolador STM32 (serie F401 o similar)
- Módulo de sensores reflectivos
- Driver de motores y motoreductores
- Fuente de alimentación estable

## 🔧 Posibles Extensiones

- Incorporación de control PID para suavizar trayectorias
- Grabación de sesiones de sensores en CSV
- Visualización en tiempo real mediante gráficas
- Transmisión inalámbrica de datos (Bluetooth, WiFi)
- Implementación de lógica para curvas o intersecciones

# Créditos

Josh Sebastián López Murcia

Julián Humberto Lozada Silva
