/*

 * SISTEMA DE GESTION DE UN ESTUDIO MUSICAL
 * Curso : Estructura de Datos - Grupo 2 - NRC 39606
 
*/

// ===== SECCION 1: LIBRERIAS =====
#include <iostream>   // cin, cout, endl: entrada y salida por consola
#include <string>     // string: manejo de textos
#include <fstream>    // ifstream, ofstream: lectura y escritura de archivos
#include <sstream>    // stringstream: separar los campos de una linea
#include <cctype>     // isdigit, toupper: revisar y convertir caracteres
#include <cstdlib>    // atoi, exit: convertir texto a numero y salir
using namespace std;

// ===== SECCION 2: ESTRUCTURAS DE DATOS (NODOS) =====

// Nodo de la LISTA: guarda una sala, un equipo o un horario.
struct Recurso {
    string codigo;      // identificador unico: SA001, EQ001, HO001
    string tipo;        // "Sala", "Equipo" o "Horario"
    string nombre;      // nombre del recurso
    string estado;      // Disponible / En uso / En mantenimiento
    int capacidad;      // solo Sala: cantidad de personas
    string categoria;   // solo Equipo: microfono, interfaz, etc.
    string sala;        // solo Horario: codigo de la sala asignada
    string fecha;       // solo Horario: DD/MM/AAAA
    int horaIni;        // solo Horario: minutos desde las 00:00
    int horaFin;        // solo Horario: minutos desde las 00:00
    Recurso* siguiente; // enlace al siguiente nodo de la lista
};

// Nodo de la PILA: guarda una pista musical.
struct Pista {
    string nombre;      // nombre de la pista (no se repite)
    Pista* siguiente;   // pista registrada antes que esta
};

// Nodo de la COLA: guarda un artista en espera.
struct Artista {
    string id;          // codigo unico: AR001, AR002, ...
    string nombre;      // nombre del artista
    string genero;      // genero musical
    int duracion;       // duracion estimada de la sesion (minutos)
    Artista* sig;       // artista que esta detras en la cola
};

// Punteros de control de cada estructura (todas empiezan vacias).
Recurso* inicio = NULL;   // primer nodo de la lista
Pista*   cima   = NULL;   // pista de arriba de la pila
Artista* frente = NULL;   // primer artista de la cola
Artista* fin    = NULL;   // ultimo artista de la cola ("final")

// ===== SECCION 3: UTILIDADES Y VALIDACION DE DATOS =====

// Convierte un texto a mayusculas (para comparar sin importar el formato).
string aMayusculas(string s) {
    for (size_t i = 0; i < s.size(); i++) {
        s[i] = (char)toupper((unsigned char)s[i]);
    }
    return s;
}

// Quita los espacios del inicio y del final de un texto.
string recortar(const string& s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

// Devuelve true si el texto tiene solo digitos (y no esta vacio).
bool esNumero(const string& s) {
    if (s.empty()) return false;
    for (size_t i = 0; i < s.size(); i++) {
        if (!isdigit((unsigned char)s[i])) return false;
    }
    return true;
}

// Muestra un mensaje y lee una linea completa. Si ya no hay entrada, termina.
string leerLinea(const string& mensaje) {
    cout << mensaje;
    string s;
    if (!getline(cin, s)) {
        cout << endl << "Fin de la entrada. Cerrando el programa." << endl;
        exit(0);
    }
    return recortar(s);
}

// Pide un texto que no este vacio, sin '|' y de maximo 40 caracteres.
string leerTexto(const string& mensaje) {
    while (true) {
        string s = leerLinea(mensaje);
        if (s.empty()) cout << "Error: el dato no puede estar vacio." << endl;
        else if (s.find('|') != string::npos) cout << "Error: no se permite el caracter '|'." << endl;
        else if (s.size() > 40) cout << "Error: maximo 40 caracteres." << endl;
        else return s;
    }
}

// Pide un numero entero entre min y max. Repite hasta que sea valido.
int leerEntero(const string& mensaje, int min, int max) {
    while (true) {
        string s = leerLinea(mensaje);
        if (!esNumero(s) || s.size() > 6) {
            cout << "Error: ingrese un numero entero (solo digitos)." << endl;
            continue;
        }
        int v = atoi(s.c_str());
        if (v < min || v > max) {
            cout << "Error: el valor debe estar entre " << min << " y " << max << "." << endl;
            continue;
        }
        return v;
    }
}

// Pide "si" o "no" (cualquier combinacion de mayusculas y minusculas).
bool leerSiNo(const string& mensaje) {
    while (true) {
        string s = aMayusculas(leerLinea(mensaje));
        if (s == "SI") return true;
        if (s == "NO") return false;
        cout << "Error: responda unicamente 'si' o 'no'." << endl;
    }
}

// Revisa que una fecha tenga el formato DD/MM/AAAA y que exista en el calendario.
bool fechaValida(const string& f) {
    if (f.size() != 10 || f[2] != '/' || f[5] != '/') return false;
    string d = f.substr(0, 2), m = f.substr(3, 2), a = f.substr(6, 4);
    if (!esNumero(d) || !esNumero(m) || !esNumero(a)) return false;
    int dia = atoi(d.c_str()), mes = atoi(m.c_str()), anio = atoi(a.c_str());
    if (anio < 2026 || anio > 2100 || mes < 1 || mes > 12 || dia < 1) return false;
    int dias[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    bool bisiesto = (anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0);
    int maximo = dias[mes - 1];
    if (mes == 2 && bisiesto) maximo = 29;
    return dia <= maximo;
}

// Pide una fecha valida (DD/MM/AAAA).
string leerFecha() {
    while (true) {
        string f = leerLinea("Fecha (DD/MM/AAAA): ");
        if (fechaValida(f)) return f;
        cout << "Error: fecha no valida. Ejemplo: 15/11/2026." << endl;
    }
}

// Pide una hora en formato HH:MM y la devuelve en minutos desde las 00:00.
int leerHora(const string& mensaje) {
    while (true) {
        string h = leerLinea(mensaje);
        if (h.size() == 5 && h[2] == ':' && esNumero(h.substr(0, 2)) && esNumero(h.substr(3, 2))) {
            int hh = atoi(h.substr(0, 2).c_str());
            int mm = atoi(h.substr(3, 2).c_str());
            if (hh < 24 && mm < 60) return hh * 60 + mm;
        }
        cout << "Error: hora no valida. Use el formato HH:MM (ejemplo 09:30)." << endl;
    }
}

// Convierte minutos desde las 00:00 al texto HH:MM.
string formatoHora(int minutos) {
    stringstream ss;
    int hh = minutos / 60, mm = minutos % 60;
    if (hh < 10) ss << "0";
    ss << hh << ":";
    if (mm < 10) ss << "0";
    ss << mm;
    return ss.str();
}

// Verifica que un codigo tenga el formato PREFIJO + 3 digitos (ejemplo SA001).
bool codigoConFormato(const string& c, const string& prefijo) {
    return c.size() == 5 && c.substr(0, 2) == prefijo && esNumero(c.substr(2, 3));
}

// Pide un codigo con el prefijo indicado. Devuelve "" si el usuario escribe 0.
string leerCodigo(const string& prefijo) {
    while (true) {
        string c = aMayusculas(leerLinea("Codigo (ejemplo " + prefijo + "001, 0 para cancelar): "));
        if (c == "0") return "";
        if (codigoConFormato(c, prefijo)) return c;
        cout << "Error: el codigo debe ser " << prefijo << " seguido de 3 numeros." << endl;
    }
}

// Pide el codigo de cualquier recurso (SA, EQ o HO). Devuelve "" si escribe 0.
string leerCodigoRecurso() {
    while (true) {
        string c = aMayusculas(leerLinea("Codigo del recurso (0 para cancelar): "));
        if (c == "0") return "";
        if (codigoConFormato(c, "SA") || codigoConFormato(c, "EQ") || codigoConFormato(c, "HO")) return c;
        cout << "Error: use SA###, EQ### o HO### (ejemplo SA001)." << endl;
    }
}

