// ===== SECCION 4: MODULO RECURSOS (LISTA ENLAZADA) =====

// Recorre la lista comparando codigos. Devuelve el nodo o NULL si no existe.
Recurso* buscarRecurso(const string& codigo) {
    Recurso* aux = inicio;
    while (aux != NULL) {
        if (aux->codigo == codigo) return aux;
        aux = aux->siguiente;
    }
    return NULL;
}

// Crea un nodo nuevo con los valores numericos en cero.
Recurso* crearNodoRecurso() {
    Recurso* r = new Recurso;
    r->capacidad = 0;
    r->horaIni = 0;
    r->horaFin = 0;
    r->siguiente = NULL;
    return r;
}

// Inserta un nodo al final de la lista.
void insertarFinal(Recurso* nuevo) {
    if (inicio == NULL) {
        inicio = nuevo;
    } else {
        Recurso* aux = inicio;
        while (aux->siguiente != NULL) aux = aux->siguiente;
        aux->siguiente = nuevo;
    }
}

// Muestra por pantalla los datos de un recurso segun su tipo.
void mostrarRecurso(Recurso* r) {
    cout << "  Codigo   : " << r->codigo << endl;
    cout << "  Tipo     : " << r->tipo << endl;
    if (r->tipo == "Sala") {
        cout << "  Nombre   : " << r->nombre << endl;
        cout << "  Capacidad: " << r->capacidad << " personas" << endl;
    } else if (r->tipo == "Equipo") {
        cout << "  Nombre   : " << r->nombre << endl;
        cout << "  Categoria: " << r->categoria << endl;
    } else {
        cout << "  Sala     : " << r->sala << endl;
        cout << "  Fecha    : " << r->fecha << endl;
        cout << "  Horario  : " << formatoHora(r->horaIni) << " - " << formatoHora(r->horaFin) << endl;
    }
    cout << "  Estado   : " << r->estado << endl;
}

// Pide el estado del recurso con un mini menu.
string leerEstado() {
    cout << "Estado: 1. Disponible  2. En uso  3. En mantenimiento" << endl;
    int o = leerEntero("Elija el estado (1-3): ", 1, 3);
    if (o == 1) return "Disponible";
    if (o == 2) return "En uso";
    return "En mantenimiento";
}

// Guarda toda la lista en recursos.txt (un recurso por linea, campos con '|').
void guardarRecursos() {
    ofstream f("recursos.txt");
    if (!f) {
        cout << "Aviso: no se pudo guardar recursos.txt." << endl;
        return;
    }
    for (Recurso* r = inicio; r != NULL; r = r->siguiente) {
        f << r->codigo << "|" << r->tipo << "|" << r->nombre << "|" << r->estado << "|"
          << r->capacidad << "|" << r->categoria << "|" << r->sala << "|" << r->fecha << "|"
          << r->horaIni << "|" << r->horaFin << endl;
    }
}

// Lee recursos.txt (si existe) y reconstruye la lista. Devuelve cuantos cargo.
int cargarRecursos() {
    ifstream f("recursos.txt");
    if (!f) return 0;
    int total = 0;
    string linea;
    while (getline(f, linea)) {
        string campo[10];
        stringstream ss(linea);
        int n = 0;
        string c;
        while (n < 10 && getline(ss, c, '|')) campo[n++] = c;
        if (n < 10) continue;                       // linea incompleta: se ignora
        Recurso* r = crearNodoRecurso();
        r->codigo = campo[0];  r->tipo = campo[1];  r->nombre = campo[2];  r->estado = campo[3];
        r->capacidad = atoi(campo[4].c_str());
        r->categoria = campo[5];  r->sala = campo[6];  r->fecha = campo[7];
        r->horaIni = atoi(campo[8].c_str());
        r->horaFin = atoi(campo[9].c_str());
        insertarFinal(r);
        total++;
    }
    return total;
}

// Devuelve true si el horario [ini, fin) se cruza con otro de la misma sala y fecha.
bool hayCruce(const string& sala, const string& fecha, int ini, int fi, Recurso* excluir) {
    for (Recurso* r = inicio; r != NULL; r = r->siguiente) {
        if (r == excluir || r->tipo != "Horario") continue;
        if (r->sala == sala && r->fecha == fecha && ini < r->horaFin && r->horaIni < fi) return true;
    }
    return false;
}

// Devuelve true si algun horario usa la sala indicada.
bool salaTieneHorarios(const string& codigoSala) {
    for (Recurso* r = inicio; r != NULL; r = r->siguiente) {
        if (r->tipo == "Horario" && r->sala == codigoSala) return true;
    }
    return false;
}

// Pide fecha y horas de un horario hasta que sean validas y no se crucen.
// Devuelve false si el usuario decide cancelar.
bool leerDatosHorario(const string& sala, string& fecha, int& ini, int& fi, Recurso* excluir) {
    while (true) {
        fecha = leerFecha();
        ini = leerHora("Hora de inicio (HH:MM): ");
        fi = leerHora("Hora de fin (HH:MM): ");
        string error = "";
        if (fi <= ini) error = "Error: la hora de fin debe ser mayor que la de inicio.";
        else if (hayCruce(sala, fecha, ini, fi, excluir)) error = "Error: el horario se cruza con otro registrado para esa sala.";
        if (error == "") return true;
        cout << error << endl;
        if (!leerSiNo("Desea intentar con otros datos? (si/no): ")) return false;
    }
}

// RF01, RF02, RF03: registra una sala, un equipo o un horario.
void insertarRecurso() {
    cout << endl << "--- REGISTRAR RECURSO ---" << endl;
    cout << "1. Sala   2. Equipo   3. Horario" << endl;
    int tipo = leerEntero("Que desea registrar (1-3): ", 1, 3);
    string prefijo = (tipo == 1) ? "SA" : (tipo == 2) ? "EQ" : "HO";

    // Codigo: formato correcto y no repetido.
    string codigo;
    while (true) {
        codigo = leerCodigo(prefijo);
        if (codigo == "") { cout << "Registro cancelado." << endl; return; }
        if (buscarRecurso(codigo) != NULL) cout << "Error: el codigo " << codigo << " ya existe." << endl;
        else break;
    }

    Recurso* nuevo = crearNodoRecurso();
    nuevo->codigo = codigo;
    if (tipo == 1) {
        nuevo->tipo = "Sala";
        nuevo->nombre = leerTexto("Nombre de la sala: ");
        nuevo->capacidad = leerEntero("Capacidad (1-100 personas): ", 1, 100);
    } else if (tipo == 2) {
        nuevo->tipo = "Equipo";
        nuevo->nombre = leerTexto("Nombre del equipo: ");
        nuevo->categoria = leerTexto("Categoria (microfono, interfaz, etc.): ");
    } else {
        nuevo->tipo = "Horario";
        // El horario debe pertenecer a una sala ya registrada.
        while (true) {
            string cs = leerCodigo("SA");
            if (cs == "") { cout << "Registro cancelado." << endl; delete nuevo; return; }
            Recurso* s = buscarRecurso(cs);
            if (s == NULL) cout << "Error: la sala " << cs << " no esta registrada." << endl;
            else { nuevo->sala = cs; break; }
        }
        nuevo->nombre = "Horario " + nuevo->sala;
        if (!leerDatosHorario(nuevo->sala, nuevo->fecha, nuevo->horaIni, nuevo->horaFin, NULL)) {
            cout << "Registro cancelado." << endl;
            delete nuevo;
            return;
        }
    }
    nuevo->estado = leerEstado();
    insertarFinal(nuevo);
    guardarRecursos();
    cout << "Registro agregado correctamente." << endl;
}

// RF04: recorre la lista y muestra todos los recursos.
void mostrarRecursos() {
    cout << endl << "--- RECURSOS REGISTRADOS ---" << endl;
    if (inicio == NULL) {
        cout << "No hay recursos registrados." << endl;
        return;
    }
    int n = 0;
    for (Recurso* r = inicio; r != NULL; r = r->siguiente) {
        mostrarRecurso(r);
        cout << "  ------------------------------" << endl;
        n++;
    }
    cout << "Total de recursos: " << n << endl;
}

// RF05: busca un recurso por su codigo.
void buscarRecursoMenu() {
    cout << endl << "--- BUSCAR RECURSO ---" << endl;
    string codigo = leerCodigoRecurso();
    if (codigo == "") { cout << "Busqueda cancelada." << endl; return; }
    Recurso* r = buscarRecurso(codigo);
    if (r == NULL) cout << "Registro no encontrado." << endl;
    else mostrarRecurso(r);
}

// RF06: modifica los datos de un recurso existente.
void actualizarRecurso() {
    cout << endl << "--- ACTUALIZAR RECURSO ---" << endl;
    string codigo = leerCodigoRecurso();
    if (codigo == "") { cout << "Actualizacion cancelada." << endl; return; }
    Recurso* r = buscarRecurso(codigo);
    if (r == NULL) { cout << "Registro no encontrado." << endl; return; }
    cout << "Datos actuales:" << endl;
    mostrarRecurso(r);

    if (r->tipo == "Horario") {
        cout << "1. Cambiar fecha y horas   2. Cambiar estado" << endl;
        int o = leerEntero("Que desea modificar (1-2): ", 1, 2);
        if (o == 1) {
            string f; int ini, fi;
            if (!leerDatosHorario(r->sala, f, ini, fi, r)) {
                cout << "Actualizacion cancelada." << endl;
                return;
            }
            r->fecha = f; r->horaIni = ini; r->horaFin = fi;
        } else {
            r->estado = leerEstado();
        }
    } else {
        string detalle = (r->tipo == "Sala") ? "la capacidad" : "la categoria";
        cout << "1. Cambiar el nombre   2. Cambiar " << detalle << "   3. Cambiar estado" << endl;
        int o = leerEntero("Que desea modificar (1-3): ", 1, 3);
        if (o == 1) r->nombre = leerTexto("Nuevo nombre: ");
        else if (o == 2 && r->tipo == "Sala") r->capacidad = leerEntero("Nueva capacidad (1-100): ", 1, 100);
        else if (o == 2) r->categoria = leerTexto("Nueva categoria: ");
        else r->estado = leerEstado();
    }
    guardarRecursos();
    cout << "Registro actualizado correctamente." << endl;
}

// RF07: elimina un recurso de la lista y libera su memoria.
void eliminarRecurso() {
    cout << endl << "--- ELIMINAR RECURSO ---" << endl;
    string codigo = leerCodigoRecurso();
    if (codigo == "") { cout << "Eliminacion cancelada." << endl; return; }
    Recurso* r = buscarRecurso(codigo);
    if (r == NULL) { cout << "Registro no encontrado." << endl; return; }
    mostrarRecurso(r);
    if (r->tipo == "Sala" && salaTieneHorarios(r->codigo)) {
        cout << "Error: la sala tiene horarios registrados. Elimine primero esos horarios." << endl;
        return;
    }
    if (!leerSiNo("Confirma la eliminacion? (si/no): ")) {
        cout << "Eliminacion cancelada. No se modifico nada." << endl;
        return;
    }
    // Se ubica el nodo anterior para "saltar" el nodo eliminado.
    if (r == inicio) {
        inicio = r->siguiente;
    } else {
        Recurso* previo = inicio;
        while (previo->siguiente != r) previo = previo->siguiente;
        previo->siguiente = r->siguiente;
    }
    delete r;                       // se libera la memoria del nodo
    guardarRecursos();
    cout << "Registro eliminado correctamente." << endl;
}

