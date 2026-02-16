/*
 * MIT License
 *
 * Copyright (c) 2025 Roberto Charreton
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * In addition, any project or software that uses this library or class must include
 * the following acknowledgment in the credits:
 *
 * "This project uses software developed by Roberto Charreton and Attribute Overload."
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/
#pragma once

/**
 * @namespace EU
 * @brief Espacio de nombres de Engine Utilities (Utilidades del Motor).
 *
 * Contiene funciones matemáticas, constantes y ayudas genéricas independientes
 * de la plataforma para operaciones aritméticas, trigonométricas y geométricas.
 */
namespace EU {

    // --- Constantes matemáticas ---
    constexpr float PI = 3.14159265358979323846f;
    constexpr float E = 2.71828182845904523536f;

    /**
     * @brief Calcula la raíz cuadrada utilizando el método de Newton-Raphson.
     *
     * Una implementación manual iterativa para aproximar la raíz.
     *
     * @param value El valor del cual calcular la raíz (debe ser >= 0).
     * @return La raíz cuadrada calculada. Retorna 0 si el valor es negativo.
     */
    inline float sqrt(float value) {
        if (value < 0) {
            return 0; // Manejo seguro de entrada negativa.
        }
        float x = value;
        float y = 1.0f;
        float epsilon = 0.00001f; // Precisión deseada.
        while (x - y > epsilon) {
            x = (x + y) / 2.0f;
            y = value / x;
        }
        return x;
    }

    /**
     * @brief Calcula el cuadrado de un número.
     * @param value El valor base.
     * @return El resultado de value * value.
     */
    inline float square(float value) {
        return value * value;
    }

    /**
     * @brief Calcula el cubo de un número.
     * @param value El valor base.
     * @return El resultado de value * value * value.
     */
    inline float cube(float value) {
        return value * value * value;
    }

    /**
     * @brief Calcula la potencia de un número usando exponenciación rápida (binaria).
     *
     * Algoritmo optimizado O(log n) para exponentes enteros.
     *
     * @param base La base de la potencia.
     * @param exponent El exponente entero. Puede ser negativo.
     * @return La base elevada al exponente.
     */
    inline float power(float base, int exponent) {
        if (exponent == 0) return 1;
        if (exponent < 0) return 1.0f / power(base, -exponent);
        float result = 1;
        while (exponent) {
            if (exponent % 2 == 1) result *= base;
            base *= base;
            exponent /= 2;
        }
        return result;
    }

    /**
     * @brief Calcula el valor absoluto de un número flotante.
     * @param value El valor de entrada.
     * @return El valor positivo de la entrada.
     */
    inline float abs(float value) {
        return (value < 0) ? -value : value;
    }

    /**
     * @brief Devuelve el máximo de dos números flotantes.
     * @param a Primer valor.
     * @param b Segundo valor.
     * @return El valor mayor entre a y b.
     */
    inline float EMax(float a, float b) {
        return (a > b) ? a : b;
    }

    /**
     * @brief Devuelve el mínimo de dos números flotantes.
     * @param a Primer valor.
     * @param b Segundo valor.
     * @return El valor menor entre a y b.
     */
    inline float EMin(float a, float b) {
        return (a < b) ? a : b;
    }

    /**
     * @brief Redondea un número al entero más cercano (Round half up).
     * @param value Valor flotante a redondear.
     * @return Valor redondeado.
     */
    inline float round(float value) {
        return (value > 0) ? static_cast<int>(value + 0.5f) : static_cast<int>(value - 0.5f);
    }

    /**
     * @brief Trunca un número hacia abajo (función suelo).
     * @param value Valor flotante.
     * @return El entero más grande menor o igual a value.
     */
    inline float floor(float value) {
        int intValue = static_cast<int>(value);
        return (value < intValue) ? intValue - 1 : intValue;
    }

    /**
     * @brief Redondea un número hacia arriba (función techo).
     * @param value Valor flotante.
     * @return El entero más pequeño mayor o igual a value.
     */
    inline float ceil(float value) {
        int intValue = static_cast<int>(value);
        return (value > intValue) ? intValue + 1 : intValue;
    }

    /**
     * @brief Versión explícita para valor absoluto de flotantes (compatible con std::fabs).
     * @param value Valor flotante.
     * @return Valor absoluto.
     */
    inline float fabs(float value) {
        return value < 0.0f ? -value : value;
    }

    // --- Funciones Trigonométricas (Aproximación por Series de Taylor) ---

    /**
     * @brief Calcula el seno de un ángulo.
     *
     * Utiliza la Serie de Taylor para aproximar el resultado.
     * @param angle Ángulo en radianes.
     * @return Seno del ángulo.
     */
    inline float sin(float angle) {
        float result = 0.0f;
        float term = angle;
        float angle_squared = angle * angle;
        int n = 1;
        // Iterar hasta alcanzar la precisión deseada
        while (term > 1e-6f || term < -1e-6f) {
            result += term;
            term *= -angle_squared / ((2 * n) * (2 * n + 1));
            ++n;
        }
        return result;
    }

    /**
     * @brief Calcula el coseno de un ángulo.
     * * Se basa en la identidad cos(x) = sin(x + PI/2).
     * @param angle Ángulo en radianes.
     * @return Coseno del ángulo.
     */
    inline float cos(float angle) {
        return sin(angle + PI / 2);
    }

    /**
     * @brief Calcula la tangente de un ángulo.
     * @param angle Ángulo en radianes.
     * @return Tangente del ángulo. Retorna 0.0f si el coseno es 0 para evitar división por cero.
     */
    inline float tan(float angle) {
        float s = sin(angle);
        float c = cos(angle);
        return c != 0.0f ? s / c : 0.0f;
    }

    /**
     * @brief Calcula el arco seno (inversa del seno).
     * * Utiliza aproximación por Serie de Taylor.
     * @param value Valor en el rango [-1, 1].
     * @return Ángulo en radianes.
     */
    inline float asin(float value) {
        float x = value;
        float result = x;
        float term = x;
        for (int n = 1; n < 10; ++n) {
            term *= x * x * (2 * n - 1) / ((2 * n + 1) * (2 * n));
            result += term;
        }
        return result;
    }

    /**
     * @brief Calcula el arco coseno.
     * * Se basa en la identidad acos(x) = PI/2 - asin(x).
     * @param value Valor en el rango [-1, 1].
     * @return Ángulo en radianes.
     */
    inline float acos(float value) {
        return PI / 2 - asin(value);
    }

    /**
     * @brief Calcula el arco tangente.
     * @param value Valor de entrada.
     * @return Ángulo en radianes en el rango [-PI/2, PI/2].
     */
    inline float atan(float value) {
        float result = 0.0f;
        float term = value;
        for (int n = 0; n < 10; ++n) {
            result += term / (2 * n + 1);
            term *= -value * value;
        }
        return result;
    }

    // --- Funciones Hiperbólicas ---

    /**
     * @brief Calcula el seno hiperbólico (sinh).
     * Formula: (e^x - e^-x) / 2
     */
    inline float sinh(float value) {
        return (exp(value) - exp(-value)) / 2;
    }

    /**
     * @brief Calcula el coseno hiperbólico (cosh).
     * Formula: (e^x + e^-x) / 2
     */
    inline float cosh(float value) {
        return (exp(value) + exp(-value)) / 2;
    }

    /**
     * @brief Calcula la tangente hiperbólica (tanh).
     * Formula: sinh(x) / cosh(x)
     */
    inline float tanh(float value) {
        return sinh(value) / cosh(value);
    }

    // --- Conversión de Ángulos ---

    /**
     * @brief Convierte grados sexagesimales a radianes.
     * @param degrees Ángulo en grados.
     * @return Ángulo en radianes.
     */
    inline float radians(float degrees) {
        return degrees * PI / 180.0f;
    }

    /**
     * @brief Convierte radianes a grados sexagesimales.
     * @param radians Ángulo en radianes.
     * @return Ángulo en grados.
     */
    inline float degrees(float radians) {
        return radians * 180.0f / PI;
    }

    // --- Funciones Exponenciales y Logarítmicas ---

    /**
     * @brief Calcula la exponencial natural e^x.
     * * Utiliza la serie de Taylor para la aproximación.
     * @param value Exponente x.
     * @return Resultado de e elevado a la potencia value.
     */
    inline float exp(float value) {
        float result = 1.0f;
        float term = 1.0f;
        for (int n = 1; n < 20; ++n) {
            term *= value / n;
            result += term;
        }
        return result;
    }

    /**
     * @brief Calcula el logaritmo natural (base e).
     * * Utiliza una aproximación por series de Taylor para (x-1)/(x+1).
     * @param value Valor de entrada (debe ser > 0).
     * @return Logaritmo natural de value. Retorna 0 si value <= 0.
     */
    inline float log(float value) {
        if (value <= 0) return 0;
        float x = (value - 1) / (value + 1);
        float x2 = x * x;
        float result = 0.0f;
        float term = x;
        for (int n = 1; n < 20; ++n) {
            result += term / (2 * n - 1);
            term *= x2;
        }
        return 2 * result;
    }

    /**
     * @brief Calcula el logaritmo en base 10.
     * @param value Valor de entrada.
     * @return Logaritmo base 10.
     */
    inline float log10(float value) {
        return log(value) / log(10);
    }

    /**
     * @brief Calcula el módulo o residuo de una división flotante.
     * @param a Dividendo.
     * @param b Divisor.
     * @return Residuo de a / b.
     */
    inline float mod(float a, float b) {
        return a - b * static_cast<int>(a / b);
    }

    // --- Funciones de Geometría Básica ---

    /**
     * @brief Calcula el área de un círculo.
     * @param radius Radio del círculo.
     */
    inline float circleArea(float radius) {
        return PI * radius * radius;
    }

    /**
     * @brief Calcula la circunferencia (perímetro) de un círculo.
     * @param radius Radio del círculo.
     */
    inline float circleCircumference(float radius) {
        return 2 * PI * radius;
    }

    /**
     * @brief Calcula el área de un rectángulo.
     * @param width Ancho.
     * @param height Alto.
     */
    inline float rectangleArea(float width, float height) {
        return width * height;
    }

    /**
     * @brief Calcula el perímetro de un rectángulo.
     * @param width Ancho.
     * @param height Alto.
     */
    inline float rectanglePerimeter(float width, float height) {
        return 2 * (width + height);
    }

    /**
     * @brief Calcula el área de un triángulo.
     * @param base Longitud de la base.
     * @param height Altura perpendicular a la base.
     */
    inline float triangleArea(float base, float height) {
        return 0.5f * base * height;
    }

    /**
     * @brief Calcula la distancia Euclidiana entre dos puntos 2D.
     * @param x1, y1 Coordenadas del primer punto.
     * @param x2, y2 Coordenadas del segundo punto.
     * @return Distancia escalar entre los puntos.
     */
    inline float distance(float x1, float y1, float x2, float y2) {
        float dx = x2 - x1;
        float dy = y2 - y1;
        return sqrt(dx * dx + dy * dy);
    }

    // --- Utilidades Numéricas ---

    /**
     * @brief Interpolación Lineal (Lerp) entre dos valores.
     * * Fórmula: a + t * (b - a).
     * @param a Valor inicial (t=0).
     * @param b Valor final (t=1).
     * @param t Factor de interpolación (generalmente entre 0.0 y 1.0).
     * @return Valor interpolado.
     */
    inline float lerp(float a, float b, float t) {
        return a + t * (b - a);
    }

    /**
     * @brief Calcula el factorial de un número entero.
     * @param n Número no negativo.
     * @return n! (1 * 2 * ... * n).
     */
    inline int factorial(int n) {
        int result = 1;
        for (int i = 2; i <= n; ++i) {
            result *= i;
        }
        return result;
    }

    /**
     * @brief Compara si dos flotantes son aproximadamente iguales.
     * * Útil para evitar errores de precisión en comparaciones de punto flotante.
     * @param a Primer valor.
     * @param b Segundo valor.
     * @param epsilon Margen de error tolerado (ej. 0.0001f).
     * @return `true` si la diferencia absoluta es menor que epsilon.
     */
    inline bool approxEqual(float a, float b, float epsilon) {
        return fabs(a - b) < epsilon;
    }

}