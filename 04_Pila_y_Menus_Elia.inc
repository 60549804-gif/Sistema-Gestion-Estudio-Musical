// ===== SECCION 6: MODULO PISTAS (PILA - LIFO) =====

// Pila vacia: la cima no apunta a ningun nodo.
bool pilaVacia(Pista* p) {
    return p == NULL;
}

// push: apila una pista en la cima.
void push(Pista*& p, const string& nombre) {
    Pista* nuevo = new Pista;
    nuevo->nombre = nombre;
    nuevo->siguiente = p;
    p = nuevo;
}

// pop: elimina la pista de la cima y libera su memoria.
void pop(Pista*& p) {
    if (p != NULL) {
        Pista* temp = p;
        p = p->siguiente;
        delete temp;
    }
}

// top: devuelve el nombre de la pista de la cima sin eliminarla.
string top(Pista* p) {
    if (p == NULL) return "";
    return p->nombre;
}

// Busca un nombre en la pila usando una pila auxiliar (solo push/pop/top),
// y al final devuelve todas las pistas a la pila original en su mismo orden.
bool existePista(const string& nombre) {
    Pista* aux = NULL;
    bool encontrada = false;
    string buscado = aMayusculas(nombre);
    while (!pilaVacia(cima)) {
        if (aMayusculas(top(cima)) == buscado) encontrada = true;
        push(aux, top(cima));
        pop(cima);
    }
    while (!pilaVacia(aux)) {
        push(cima, top(aux));
        pop(aux);
    }
    return encontrada;
}

// Escribe la pila desde el fondo hasta la cima (asi, al cargar, se apila igual).
void escribirPilaRec(Pista* p, ofstream& f) {
    if (p == NULL) return;
    escribirPilaRec(p->siguiente, f);
    f << p->nombre << endl;
}

// Guarda la pila en pistas.txt.
void guardarPistas() {
    ofstream f("pistas.txt");
    if (!f) {
        cout << "Aviso: no se pudo guardar pistas.txt." << endl;
        return;
    }
    escribirPilaRec(cima, f);
}

// Lee pistas.txt (si existe) y reconstruye la pila. Devuelve cuantas cargo.
int cargarPistas() {
    ifstream f("pistas.txt");
    if (!f) return 0;
    int total = 0;
    string linea;
    while (getline(f, linea)) {
        linea = recortar(linea);
        if (linea.empty()) continue;
        push(cima, linea);
        total++;
    }
    return total;
}

// RF11: registra una pista nueva (no se permiten nombres repetidos).
void registrarPista() {
    cout << endl << "--- REGISTRAR PISTA MUSICAL ---" << endl;
    while (true) {
        string nombre = leerLinea("Ingresar nombre de pista musical (0 para cancelar): ");
        if (nombre == "0") { cout << "Registro cancelado." << endl; return; }
        if (nombre.empty()) { cout << "Error: el nombre no puede estar vacio." << endl; continue; }
        if (nombre.find('|') != string::npos) { cout << "Error: no se permite el caracter '|'." << endl; continue; }
        if (nombre.size() > 40) { cout << "Error: maximo 40 caracteres." << endl; continue; }
        if (existePista(nombre)) {
            cout << "Nombre de pista ya registrada... Modificar nombre" << endl;
            continue;
        }
        push(cima, nombre);
        guardarPistas();
        cout << "Pista registrada correctamente." << endl;
        return;
    }
}

// RF12: muestra la ultima pista registrada (la cima) sin eliminarla.
void consultarUltimaPista() {
    cout << endl << "--- ULTIMA PISTA ---" << endl;
    if (pilaVacia(cima)) {
        cout << "No hay pistas registradas." << endl;
        return;
    }
    cout << "La ultima pista agregada es: " << top(cima) << endl;
}

// RF13: elimina la ultima pista registrada, previa confirmacion.
void eliminarUltimaPista() {
    cout << endl << "--- ELIMINAR ULTIMA PISTA ---" << endl;
    if (pilaVacia(cima)) {
        cout << "No hay pistas registradas." << endl;
        return;
    }
    if (!leerSiNo("Desea eliminar \"" + top(cima) + "\"? (si/no): ")) {
        cout << "Eliminacion cancelada. No se modifico nada." << endl;
        return;
    }
    pop(cima);
    guardarPistas();
    cout << "Pista eliminada correctamente." << endl;
}

// Muestra todas las pistas de la cima al fondo usando una pila auxiliar.
void mostrarPistas() {
    cout << endl << "--- PISTAS REGISTRADAS ---" << endl;
    if (pilaVacia(cima)) {
        cout << "No hay pistas registradas." << endl;
        return;
    }
    Pista* aux = NULL;
    int pos = 1;
    while (!pilaVacia(cima)) {
        cout << pos << ". " << top(cima);
        if (pos == 1) cout << "  (cima)";
        cout << endl;
        push(aux, top(cima));
        pop(cima);
        pos++;
    }
    while (!pilaVacia(aux)) {       // se restaura la pila original
        push(cima, top(aux));
        pop(aux);
    }
}

// ===== SECCION 7: MENUS Y PROGRAMA PRINCIPAL =====

// Submenu del modulo Recursos (lista enlazada).
void menuRecursos() {
    int op;
    do {
        cout << endl << "=== RECURSOS DEL ESTUDIO (LISTA) ===" << endl;
        cout << "1. Registrar recurso" << endl;
        cout << "2. Mostrar recursos" << endl;
        cout << "3. Buscar recurso" << endl;
        cout << "4. Actualizar recurso" << endl;
        cout << "5. Eliminar recurso" << endl;
        cout << "0. Volver al menu principal" << endl;
        op = leerEntero("Elija una opcion: ", 0, 5);
        if (op == 1) insertarRecurso();
        else if (op == 2) mostrarRecursos();
        else if (op == 3) buscarRecursoMenu();
        else if (op == 4) actualizarRecurso();
        else if (op == 5) eliminarRecurso();
    } while (op != 0);
}

// Submenu del modulo Artistas (cola).
void menuArtistas() {
    int op;
    do {
        cout << endl << "=== ARTISTAS EN ESPERA (COLA) ===" << endl;
        cout << "1. Agregar artista" << endl;
        cout << "2. Atender artista" << endl;
        cout << "3. Mostrar artistas pendientes" << endl;
        cout << "0. Volver al menu principal" << endl;
        op = leerEntero("Elija una opcion: ", 0, 3);
        if (op == 1) agregarArtista();
        else if (op == 2) atenderArtista();
        else if (op == 3) mostrarArtistas();
    } while (op != 0);
}

// Submenu del modulo Pistas (pila).
void menuPistas() {
    int op;
    do {
        cout << endl << "=== PISTAS MUSICALES (PILA) ===" << endl;
        cout << "1. Registrar nueva pista" << endl;
        cout << "2. Consultar ultima pista" << endl;
        cout << "3. Eliminar ultima pista" << endl;
        cout << "4. Mostrar todas las pistas" << endl;
        cout << "0. Volver al menu principal" << endl;
        op = leerEntero("Elija una opcion: ", 0, 4);
        if (op == 1) registrarPista();
        else if (op == 2) consultarUltimaPista();
        else if (op == 3) eliminarUltimaPista();
        else if (op == 4) mostrarPistas();
    } while (op != 0);
}

// Libera la memoria de todos los nodos antes de terminar (RNF10).
void liberarMemoria() {
    while (inicio != NULL) {
        Recurso* temp = inicio;
        inicio = inicio->siguiente;
        delete temp;
    }
    while (frente != NULL) desencolar();
    while (!pilaVacia(cima)) pop(cima);
}

// Programa principal: carga los archivos, muestra el menu y repite hasta salir.
int main() {
    int nRec = cargarRecursos();
    int nArt = cargarArtistas();
    int nPis = cargarPistas();

    cout << "****************************************" << endl;
    cout << "  SISTEMA DE GESTION DE UN ESTUDIO MUSICAL" << endl;
    cout << "****************************************" << endl;
    cout << "Datos cargados: " << nRec << " recursos, " << nArt << " artistas, "
         << nPis << " pistas." << endl;

    int op;
    do {
        cout << endl << "========== MENU PRINCIPAL ==========" << endl;
        cout << "1. Recursos del estudio (salas, equipos, horarios)" << endl;
        cout << "2. Artistas en espera" << endl;
        cout << "3. Pistas musicales" << endl;
        cout << "4. Salir" << endl;
        op = leerEntero("Elija una opcion: ", 1, 4);
        if (op == 1) menuRecursos();
        else if (op == 2) menuArtistas();
        else if (op == 3) menuPistas();
    } while (op != 4);

    liberarMemoria();
    cout << "Datos guardados. Hasta pronto." << endl;
    leerLinea("Presione ENTER para cerrar...");
    return 0;
}