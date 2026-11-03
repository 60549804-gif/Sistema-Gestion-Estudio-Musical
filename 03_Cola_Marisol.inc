// ===== SECCION 5: MODULO ARTISTAS (COLA - FIFO) =====

// Devuelve true si ya hay un artista con ese codigo en la cola.
bool existeArtista(const string& id) {
    for (Artista* a = frente; a != NULL; a = a->sig) {
        if (a->id == id) return true;
    }
    return false;
}

// Agrega un artista al FINAL de la cola (enqueue).
void encolar(const string& id, const string& nombre, const string& genero, int duracion) {
    Artista* nuevo = new Artista;
    nuevo->id = id;
    nuevo->nombre = nombre;
    nuevo->genero = genero;
    nuevo->duracion = duracion;
    nuevo->sig = NULL;
    if (frente == NULL) {           // cola vacia: frente y fin son el mismo nodo
        frente = nuevo;
    } else {
        fin->sig = nuevo;           // se engancha detras del ultimo
    }
    fin = nuevo;
}

// Retira al artista del FRENTE de la cola (dequeue).
void desencolar() {
    if (frente == NULL) return;
    Artista* temp = frente;
    frente = frente->sig;
    if (frente == NULL) fin = NULL; // la cola quedo vacia
    delete temp;
}

// Guarda la cola en artistas.txt, del frente al final.
void guardarArtistas() {
    ofstream f("artistas.txt");
    if (!f) {
        cout << "Aviso: no se pudo guardar artistas.txt." << endl;
        return;
    }
    for (Artista* a = frente; a != NULL; a = a->sig) {
        f << a->id << "|" << a->nombre << "|" << a->genero << "|" << a->duracion << endl;
    }
}

// Lee artistas.txt (si existe) y reconstruye la cola. Devuelve cuantos cargo.
int cargarArtistas() {
    ifstream f("artistas.txt");
    if (!f) return 0;
    int total = 0;
    string linea;
    while (getline(f, linea)) {
        string campo[4];
        stringstream ss(linea);
        int n = 0;
        string c;
        while (n < 4 && getline(ss, c, '|')) campo[n++] = c;
        if (n < 4) continue;
        encolar(campo[0], campo[1], campo[2], atoi(campo[3].c_str()));
        total++;
    }
    return total;
}

// RF08: registra un artista al final de la cola de espera.
void agregarArtista() {
    cout << endl << "--- AGREGAR ARTISTA A LA COLA ---" << endl;
    string id;
    while (true) {
        id = leerCodigo("AR");
        if (id == "") { cout << "Registro cancelado." << endl; return; }
        if (existeArtista(id)) cout << "Error: el codigo " << id << " ya esta en la cola." << endl;
        else break;
    }
    string nombre = leerTexto("Nombre del artista: ");
    string genero = leerTexto("Genero musical: ");
    int duracion = leerEntero("Duracion estimada de la sesion en minutos (15-480): ", 15, 480);
    encolar(id, nombre, genero, duracion);
    guardarArtistas();
    cout << "Artista agregado correctamente." << endl;
}

// RF09: atiende (retira) al artista que esta al frente de la cola.
void atenderArtista() {
    cout << endl << "--- ATENDER ARTISTA ---" << endl;
    if (frente == NULL) {
        cout << "No hay artistas en espera." << endl;
        return;
    }
    cout << "Artista al frente de la cola:" << endl;
    cout << "  Codigo  : " << frente->id << endl;
    cout << "  Nombre  : " << frente->nombre << endl;
    cout << "  Genero  : " << frente->genero << endl;
    cout << "  Duracion: " << frente->duracion << " min" << endl;
    if (!leerSiNo("Confirma la atencion? (si/no): ")) {
        cout << "Atencion cancelada. El artista sigue en la cola." << endl;
        return;
    }
    desencolar();
    guardarArtistas();
    cout << "Artista atendido correctamente." << endl;
}

// RF10: muestra los artistas pendientes, del primero al ultimo.
void mostrarArtistas() {
    cout << endl << "--- ARTISTAS EN ESPERA ---" << endl;
    if (frente == NULL) {
        cout << "No hay artistas en la lista de espera." << endl;
        return;
    }
    int pos = 1;
    for (Artista* a = frente; a != NULL; a = a->sig) {
        cout << pos << ". " << a->id << " | " << a->nombre << " | " << a->genero
             << " | " << a->duracion << " min" << endl;
        pos++;
    }
}

