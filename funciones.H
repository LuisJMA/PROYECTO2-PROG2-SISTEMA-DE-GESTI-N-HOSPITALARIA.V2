#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <iostream>
#include <cstring>
#include <iomanip>
#include "ESTRUCTURAS.H"

using namespace std;


// ============================================================================
// FUNCIONES DE UTILIDAD
// ============================================================================

void limpiarBuffer() {
    cin.clear();
    cin.ignore(10000, '\n');
}

bool validarFecha(const char* fecha) {
    if (strlen(fecha) != 10) return false;
    if (fecha[4] != '-' || fecha[7] != '-') return false;
    
    // Extraer componentes
    int anio = atoi(fecha);
    int mes = atoi(fecha + 5);
    int dia = atoi(fecha + 8);
    
    // Validaciones básicas
    if (anio < 1900 || anio > 2100) return false;
    if (mes < 1 || mes > 12) return false;
    
    // Validar días según mes
    int diasPorMes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    // Año bisiesto
    if (mes == 2 && (anio % 4 == 0 && (anio % 100 != 0 || anio % 400 == 0))) {
        if (dia < 1 || dia > 29) return false;
    } else {
        if (dia < 1 || dia > diasPorMes[mes-1]) return false;
    }
    
    return true;
}

bool validarHora(const char* hora) {
    if (strlen(hora) != 5) return false;
    if (hora[2] != ':') return false;
    
    int horas = atoi(hora);
    int minutos = atoi(hora + 3);
    
    if (horas < 0 || horas > 23) return false;
    if (minutos < 0 || minutos > 59) return false;
    
    return true;
}

void mostrarError(const char* mensaje) {
    cout << "* Error: " << mensaje << endl;
}

void mostrarExito(const char* mensaje) {
    cout << "* " << mensaje << endl;
}

void mostrarInfo(const char* mensaje) {
    cout << "* " << mensaje << endl;
}

// ============================================================================
// SISTEMA DE ARCHIVOS BINARIOS 
// ============================================================================

// FUNCIÓN: Inicializar un archivo binario 
bool inicializarArchivo(const char* nombreArchivo) {
    fstream archivo(nombreArchivo, ios::binary | ios::out);
    if (!archivo.is_open()) {
        mostrarError("No se pudo crear el archivo");
        return false;
    }
    
    ArchivoHeader header;
    header.cantidadRegistros = 0;
    header.proximoID = 1;
    header.registrosActivos = 0;
    header.version = VERSION_ACTUAL;
    
    archivo.write((char*)&header, sizeof(ArchivoHeader));
    archivo.close();
    
    cout << "* Archivo " << nombreArchivo << " inicializado correctamente." << endl;
    return true;
}

//  FUNCIÓN: Verificar si un archivo existe y es válido
bool verificarArchivo(const char* nombreArchivo) {
    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo.is_open()) {
        cout << "* Archivo " << nombreArchivo << " no existe, creandolo..." << endl;
        return inicializarArchivo(nombreArchivo);
    }
    
    ArchivoHeader header;
    archivo.read((char*)&header, sizeof(ArchivoHeader));
    archivo.close();
    
    if (header.version != VERSION_ACTUAL) {
        mostrarError("Version incompatible del archivo");
        return false;
    }
    
    cout << "* " << nombreArchivo << " (" << header.registrosActivos << " registros activos)" << endl;
    return true;
}

//  FUNCIÓN: Leer header de cualquier archivo
ArchivoHeader leerHeader(const char* nombreArchivo) {
    ArchivoHeader header;
    ifstream archivo(nombreArchivo, ios::binary);
    
    if (archivo.is_open()) {
        archivo.read((char*)&header, sizeof(ArchivoHeader));
        archivo.close();
    } else {
        // Header por defecto si no existe
        header.cantidadRegistros = 0;
        header.proximoID = 1;
        header.registrosActivos = 0;
        header.version = VERSION_ACTUAL;
    }
    
    return header;
}

// FUNCIÓN: Actualizar header de un archivo
bool actualizarHeader(const char* nombreArchivo, ArchivoHeader nuevoHeader) {
    fstream archivo(nombreArchivo, ios::binary | ios::in | ios::out);
    if (!archivo.is_open()) {
        return false;
    }
    
    archivo.seekp(0);
    archivo.write((char*)&nuevoHeader, sizeof(ArchivoHeader));
    archivo.close();
    return true;
}

// FUNCIÓN: Calcular posición en bytes de un registro
template<typename T>
long calcularPosicion(int indice) {
    return sizeof(ArchivoHeader) + (indice * sizeof(T));
}

// ============================================================================
// SISTEMA DE ARCHIVOS - HOSPITAL
// ============================================================================

// FUNCIÓN: Cargar datos del hospital desde archivo
bool cargarDatosHospital() {
    // Verificar que todos los archivos existan
    const char* archivos[] = {
        ARCHIVO_HOSPITAL, ARCHIVO_PACIENTES, ARCHIVO_DOCTORES, 
        ARCHIVO_CITAS, ARCHIVO_HISTORIALES
    };
    
    cout << " Verificando archivos del sistema..." << endl;
    for (int i = 0; i < 5; i++) {
        if (!verificarArchivo(archivos[i])) {
            return false;
        }
    }
    
    // Cargar datos del hospital
    ifstream archivo(ARCHIVO_HOSPITAL, ios::binary);
    
    if (archivo.is_open()) {
        archivo.read((char*)&hospitalGlobal, sizeof(Hospital));
        archivo.close();
        cout << " Hospital '" << hospitalGlobal.nombre << "' cargado." << endl;
    } else {
        // Valores por defecto
        strcpy(hospitalGlobal.nombre, "Hospital Central");
        strcpy(hospitalGlobal.direccion, "Av. Principal Milagro");
        strcpy(hospitalGlobal.telefono, "555-1234");
        hospitalGlobal.siguienteIDPaciente = 1;
        hospitalGlobal.siguienteIDDoctor = 1;
        hospitalGlobal.siguienteIDCita = 1;
        hospitalGlobal.siguienteIDConsulta = 1;
        hospitalGlobal.totalPacientesRegistrados = 0;
        hospitalGlobal.totalDoctoresRegistrados = 0;
        hospitalGlobal.totalCitasAgendadas = 0;
        hospitalGlobal.totalConsultasRealizadas = 0;
        
        cout << " Hospital creado con valores por defecto." << endl;
    }
    
    return true;
}

// FUNCIÓN: Guardar datos del hospital en archivo
bool guardarDatosHospital() {
    ofstream archivo(ARCHIVO_HOSPITAL, ios::binary);
    if (!archivo.is_open()) {
        mostrarError("No se pudo guardar datos del hospital");
        return false;
    }
    
    archivo.write((char*)&hospitalGlobal, sizeof(Hospital));
    archivo.close();
    
    mostrarExito("Datos del hospital guardados correctamente");
    return true;
}

// ============================================================================
// SISTEMA DE ARCHIVOS - PACIENTES (ACCESO ALEATORIO)
// ============================================================================

// FUNCIÓN: Buscar índice de paciente por ID
int buscarIndicePacientePorID(int id) {
    ifstream archivo(ARCHIVO_PACIENTES, ios::binary);
    ArchivoHeader header = leerHeader(ARCHIVO_PACIENTES);
    
    Paciente temp;
    for (int i = 0; i < header.cantidadRegistros; i++) {
        long posicion = calcularPosicion<Paciente>(i);
        archivo.seekg(posicion);
        archivo.read((char*)&temp, sizeof(Paciente));
        
        if (temp.id == id && !temp.eliminado) {
            archivo.close();
            return i;  // Índice encontrado
        }
    }
    
    archivo.close();
    return -1;  // No encontrado
}

//  FUNCIÓN: Leer paciente por índice (ACCESO ALEATORIO)
Paciente leerPacientePorIndice(int indice) {
    Paciente p;
    p.id = -1;  // Marcador de no encontrado
    
    ifstream archivo(ARCHIVO_PACIENTES, ios::binary);
    if (archivo.is_open()) {
        long posicion = calcularPosicion<Paciente>(indice);
        archivo.seekg(posicion);
        archivo.read((char*)&p, sizeof(Paciente));
        archivo.close();
    }
    
    return p;
}

//  FUNCIÓN: Buscar paciente por ID
Paciente buscarPacientePorID(int id) {
    int indice = buscarIndicePacientePorID(id);
    if (indice != -1) {
        return leerPacientePorIndice(indice);
    }
    
    // Retornar paciente vacío si no se encuentra
    Paciente vacio;
    vacio.id = -1;
    return vacio;
}

//  FUNCIÓN: Buscar paciente por cédula
Paciente buscarPacientePorCedula(const char* cedula) {
    ifstream archivo(ARCHIVO_PACIENTES, ios::binary);
    ArchivoHeader header = leerHeader(ARCHIVO_PACIENTES);
    
    Paciente temp;
    for (int i = 0; i < header.cantidadRegistros; i++) {
        long posicion = calcularPosicion<Paciente>(i);
        archivo.seekg(posicion);
        archivo.read((char*)&temp, sizeof(Paciente));
        
        if (strcmp(temp.cedula, cedula) == 0 && !temp.eliminado) {
            archivo.close();
            return temp;
        }
    }
    
    archivo.close();
    Paciente vacio;
    vacio.id = -1;
    return vacio;
}

// FUNCIÓN: Agregar nuevo paciente al archivo
bool agregarPaciente(Paciente nuevoPaciente) {
    fstream archivo(ARCHIVO_PACIENTES, ios::binary | ios::in | ios::out);
    if (!archivo.is_open()) {
        mostrarError("No se pudo abrir archivo de pacientes");
        return false;
    }
    
    // Leer header actual
    ArchivoHeader header;
    archivo.read((char*)&header, sizeof(ArchivoHeader));
    
    // Asignar ID y timestamps
    nuevoPaciente.id = header.proximoID;
    nuevoPaciente.fechaCreacion = time(0);
    nuevoPaciente.fechaModificacion = time(0);
    nuevoPaciente.eliminado = false;
    
    // Inicializar arrays
    for (int i = 0; i < MAX_CITAS_PACIENTE; i++) {
        nuevoPaciente.citasIDs[i] = -1;
    }
    
    // Posicionarse al final para escribir
    long posicion = calcularPosicion<Paciente>(header.cantidadRegistros);
    archivo.seekp(posicion);
    archivo.write((char*)&nuevoPaciente, sizeof(Paciente));
    
    // Actualizar header
    header.cantidadRegistros++;
    header.proximoID++;
    header.registrosActivos++;
    
    archivo.seekp(0);
    archivo.write((char*)&header, sizeof(ArchivoHeader));
    archivo.close();
    
    // Actualizar hospital global
    hospitalGlobal.siguienteIDPaciente = header.proximoID;
    hospitalGlobal.totalPacientesRegistrados = header.registrosActivos;
    
    cout << " Paciente registrado exitosamente. ID: " << nuevoPaciente.id << endl;
    return true;
}

//  FUNCIÓN: Actualizar paciente existente
bool actualizarPaciente(Paciente pacienteModificado) {
    int indice = buscarIndicePacientePorID(pacienteModificado.id);
    if (indice == -1) {
        mostrarError("Paciente no encontrado para actualizar");
        return false;
    }
    
    fstream archivo(ARCHIVO_PACIENTES, ios::binary | ios::in | ios::out);
    if (!archivo.is_open()) {
        return false;
    }
    
    // Actualizar timestamp
    pacienteModificado.fechaModificacion = time(0);
    
    // Posicionarse y sobrescribir
    long posicion = calcularPosicion<Paciente>(indice);
    archivo.seekp(posicion);
    archivo.write((char*)&pacienteModificado, sizeof(Paciente));
    archivo.close();
    
    mostrarExito("Paciente actualizado correctamente");
    return true;
}

//  FUNCIÓN: Listar todos los pacientes
void listarPacientes() {
    ifstream archivo(ARCHIVO_PACIENTES, ios::binary);
    ArchivoHeader header = leerHeader(ARCHIVO_PACIENTES);
    
    if (header.registrosActivos == 0) {
        cout << " No hay pacientes registrados en el sistema." << endl;
        return;
    }
    
    cout << "\n+------------------------------------------------------------+" << endl;
    cout << "¦                    LISTA DE PACIENTES                      ¦" << endl;
    cout << "¦------------------------------------------------------------¦" << endl;
    cout << "¦ ID  ¦ NOMBRE COMPLETO     ¦ CÉDULA       ¦ EDAD ¦ CONSULTAS¦" << endl;
    cout << "¦-----+---------------------+--------------+------+----------¦" << endl;
    
    Paciente temp;
    int contador = 0;
    
    for (int i = 0; i < header.cantidadRegistros; i++) {
        long posicion = calcularPosicion<Paciente>(i);
        archivo.seekg(posicion);
        archivo.read((char*)&temp, sizeof(Paciente));
        
        if (!temp.eliminado) {
            cout << "¦ " << setw(3) << temp.id << " ¦ "
                 << setw(19) << left << (string(temp.nombre) + " " + temp.apellido) << " ¦ "
                 << setw(12) << temp.cedula << " ¦ "
                 << setw(4) << temp.edad << " ¦ "
                 << setw(8) << temp.cantidadConsultas << "¦" << endl;
            contador++;
        }
    }
    
    archivo.close();
    
    cout << "+------------------------------------------------------------+" << endl;
    cout << "Total de pacientes activos: " << contador << endl;
}

// ============================================================================
// SISTEMA DE ARCHIVOS - DOCTORES
// ============================================================================

//  FUNCIÓN: Buscar doctor por ID
Doctor buscarDoctorPorID(int id) {
    ifstream archivo(ARCHIVO_DOCTORES, ios::binary);
    ArchivoHeader header = leerHeader(ARCHIVO_DOCTORES);
    
    Doctor temp;
    for (int i = 0; i < header.cantidadRegistros; i++) {
        long posicion = calcularPosicion<Doctor>(i);
        archivo.seekg(posicion);
        archivo.read((char*)&temp, sizeof(Doctor));
        
        if (temp.id == id && !temp.eliminado) {
            archivo.close();
            return temp;
        }
    }
    
    archivo.close();
    Doctor vacio;
    vacio.id = -1;
    return vacio;
}

//  FUNCIÓN: Agregar nuevo doctor al archivo
bool agregarDoctor(Doctor nuevoDoctor) {
    fstream archivo(ARCHIVO_DOCTORES, ios::binary | ios::in | ios::out);
    if (!archivo.is_open()) {
        mostrarError("No se pudo abrir archivo de doctores");
        return false;
    }
    
    ArchivoHeader header;
    archivo.read((char*)&header, sizeof(ArchivoHeader));
    
    nuevoDoctor.id = header.proximoID;
    nuevoDoctor.fechaCreacion = time(0);
    nuevoDoctor.fechaModificacion = time(0);
    nuevoDoctor.eliminado = false;
    
    // Inicializar arrays
    for (int i = 0; i < MAX_PACIENTES_DOCTOR; i++) {
        nuevoDoctor.pacientesIDs[i] = -1;
    }
    for (int i = 0; i < MAX_CITAS_DOCTOR; i++) {
        nuevoDoctor.citasIDs[i] = -1;
    }
    
    long posicion = calcularPosicion<Doctor>(header.cantidadRegistros);
    archivo.seekp(posicion);
    archivo.write((char*)&nuevoDoctor, sizeof(Doctor));
    
    header.cantidadRegistros++;
    header.proximoID++;
    header.registrosActivos++;
    
    archivo.seekp(0);
    archivo.write((char*)&header, sizeof(ArchivoHeader));
    archivo.close();
    
    hospitalGlobal.siguienteIDDoctor = header.proximoID;
    hospitalGlobal.totalDoctoresRegistrados = header.registrosActivos;
    
    cout << " Doctor registrado exitosamente. ID: " << nuevoDoctor.id << endl;
    return true;
}

//  FUNCIÓN: Listar todos los doctores
void listarDoctores() {
    ifstream archivo(ARCHIVO_DOCTORES, ios::binary);
    ArchivoHeader header = leerHeader(ARCHIVO_DOCTORES);
    
    if (header.registrosActivos == 0) {
        cout << " No hay doctores registrados en el sistema." << endl;
        return;
    }
    
    cout << "\n+----------------------------------------------------------------------+" << endl;
    cout << "¦                         LISTA DE DOCTORES                           ¦" << endl;
    cout << "¦---------------------------------------------------------------------¦" << endl;
    cout << "¦ ID  ¦ NOMBRE COMPLETO     ¦ ESPECIALIDAD     ¦ EXP  ¦ COSTO CONSULTA¦" << endl;
    cout << "¦-----+---------------------+------------------+------+---------------¦" << endl;
    
    Doctor temp;
    int contador = 0;
    
    for (int i = 0; i < header.cantidadRegistros; i++) {
        long posicion = calcularPosicion<Doctor>(i);
        archivo.seekg(posicion);
        archivo.read((char*)&temp, sizeof(Doctor));
        
        if (!temp.eliminado) {
            cout << "¦ " << setw(3) << temp.id << " ¦ "
                 << setw(19) << left << (string(temp.nombre) + " " + temp.apellido) << " ¦ "
                 << setw(16) << temp.especialidad << " ¦ "
                 << setw(4) << temp.aniosExperiencia << " ¦ "
                 << setw(13) << fixed << setprecision(2) << temp.costoConsulta << " ¦" << endl;
            contador++;
        }
    }
    
    archivo.close();
    
    cout << "+---------------------------------------------------------------------+" << endl;
    cout << "Total de doctores activos: " << contador << endl;
}

// ============================================================================
// SISTEMA DE ARCHIVOS - CITAS (FUNCIONALIDAD COMPLETA)
// ============================================================================

//  FUNCIÓN: Buscar índice de cita por ID
int buscarIndiceCitaPorID(int id) {
    ifstream archivo(ARCHIVO_CITAS, ios::binary);
    ArchivoHeader header = leerHeader(ARCHIVO_CITAS);
    
    Cita temp;
    for (int i = 0; i < header.cantidadRegistros; i++) {
        long posicion = calcularPosicion<Cita>(i);
        archivo.seekg(posicion);
        archivo.read((char*)&temp, sizeof(Cita));
        
        if (temp.id == id && !temp.eliminado) {
            archivo.close();
            return i;
        }
    }
    
    archivo.close();
    return -1;
}

//  FUNCIÓN: Agregar nueva cita al archivo
bool agregarCita(Cita nuevaCita) {
    fstream archivo(ARCHIVO_CITAS, ios::binary | ios::in | ios::out);
    if (!archivo.is_open()) {
        mostrarError("No se pudo abrir archivo de citas");
        return false;
    }
    
    ArchivoHeader header;
    archivo.read((char*)&header, sizeof(ArchivoHeader));
    
    nuevaCita.id = header.proximoID;
    nuevaCita.fechaCreacion = time(0);
    nuevaCita.fechaModificacion = time(0);
    nuevaCita.eliminado = false;
    nuevaCita.consultaID = -1;  // No atendida aún
    
    long posicion = calcularPosicion<Cita>(header.cantidadRegistros);
    archivo.seekp(posicion);
    archivo.write((char*)&nuevaCita, sizeof(Cita));
    
    header.cantidadRegistros++;
    header.proximoID++;
    header.registrosActivos++;
    
    archivo.seekp(0);
    archivo.write((char*)&header, sizeof(ArchivoHeader));
    archivo.close();
    
    // Actualizar hospital global
    hospitalGlobal.siguienteIDCita = header.proximoID;
    hospitalGlobal.totalCitasAgendadas = header.registrosActivos;
    
    // Agregar cita al paciente
    Paciente paciente = buscarPacientePorID(nuevaCita.pacienteID);
    if (paciente.id != -1 && paciente.cantidadCitas < MAX_CITAS_PACIENTE) {
        paciente.citasIDs[paciente.cantidadCitas] = nuevaCita.id;
        paciente.cantidadCitas++;
        actualizarPaciente(paciente);
    }
    
    // Agregar cita al doctor
    Doctor doctor = buscarDoctorPorID(nuevaCita.doctorID);
    if (doctor.id != -1 && doctor.cantidadCitas < MAX_CITAS_DOCTOR) {
        doctor.citasIDs[doctor.cantidadCitas] = nuevaCita.id;
        doctor.cantidadCitas++;
        
        // Actualizar doctor en archivo
        fstream archivoDoc(ARCHIVO_DOCTORES, ios::binary | ios::in | ios::out);
        if (archivoDoc.is_open()) {
            ArchivoHeader headerDoc = leerHeader(ARCHIVO_DOCTORES);
            Doctor tempDoc;
            
            for (int i = 0; i < headerDoc.cantidadRegistros; i++) {
                long posDoc = calcularPosicion<Doctor>(i);
                archivoDoc.seekg(posDoc);
                archivoDoc.read((char*)&tempDoc, sizeof(Doctor));
                
                if (tempDoc.id == doctor.id) {
                    tempDoc.citasIDs[tempDoc.cantidadCitas] = nuevaCita.id;
                    tempDoc.cantidadCitas++;
                    tempDoc.fechaModificacion = time(0);
                    
                    archivoDoc.seekp(posDoc);
                    archivoDoc.write((char*)&tempDoc, sizeof(Doctor));
                    break;
                }
            }
            archivoDoc.close();
        }
    }
    
    cout << "* Cita agendada exitosamente. ID: " << nuevaCita.id << endl;
    return true;
}

//  FUNCIÓN: Verificar disponibilidad de doctor
bool verificarDisponibilidad(int idDoctor, const char* fecha, const char* hora) {
    ifstream archivo(ARCHIVO_CITAS, ios::binary);
    ArchivoHeader header = leerHeader(ARCHIVO_CITAS);
    
    Cita temp;
    for (int i = 0; i < header.cantidadRegistros; i++) {
        long posicion = calcularPosicion<Cita>(i);
        archivo.seekg(posicion);
        archivo.read((char*)&temp, sizeof(Cita));
        
        // Verificar si el doctor ya tiene cita en esa fecha/hora
        if (temp.doctorID == idDoctor && 
            strcmp(temp.fecha, fecha) == 0 && 
            strcmp(temp.hora, hora) == 0 &&
            strcmp(temp.estado, "Cancelada") != 0 &&
            !temp.eliminado) {
            archivo.close();
            return false;  // No disponible
        }
    }
    
    archivo.close();
    return true;  // Disponible
}

//  FUNCIÓN: Listar citas de un paciente
void listarCitasPaciente(int pacienteID) {
    Paciente paciente = buscarPacientePorID(pacienteID);
    if (paciente.id == -1) {
        mostrarError("Paciente no encontrado");
        return;
    }
    
    if (paciente.cantidadCitas == 0) {
        cout << "** El paciente no tiene citas agendadas." << endl;
        return;
    }
    
    cout << "\n+------------------------------------------------------------------------------+" << endl;
    cout << "¦                         CITAS DEL PACIENTE                                   ¦" << endl;
    cout << "¦------------------------------------------------------------------------------¦" << endl;
    cout << "¦ ID  ¦ FECHA      ¦ HORA   ¦ DOCTOR              ¦ ESTADO         ¦ MOTIVO    ¦" << endl;
    cout << "¦-----+------------+--------+---------------------+----------------+-----------¦" << endl;
    
    for (int i = 0; i < paciente.cantidadCitas; i++) {
        int citaID = paciente.citasIDs[i];
        if (citaID != -1) {
            // Buscar cita en archivo
            ifstream archivo(ARCHIVO_CITAS, ios::binary);
            ArchivoHeader header = leerHeader(ARCHIVO_CITAS);
            
            Cita temp;
            for (int j = 0; j < header.cantidadRegistros; j++) {
                long posicion = calcularPosicion<Cita>(j);
                archivo.seekg(posicion);
                archivo.read((char*)&temp, sizeof(Cita));
                
                if (temp.id == citaID && !temp.eliminado) {
                    Doctor doctor = buscarDoctorPorID(temp.doctorID);
                    string nombreDoctor = (doctor.id != -1) ? 
                        string(doctor.nombre) + " " + doctor.apellido : "No encontrado";
                    
                    cout << "¦ " << setw(3) << temp.id << " ¦ "
                         << setw(10) << temp.fecha << " ¦ "
                         << setw(6) << temp.hora << " ¦ "
                         << setw(19) << left << nombreDoctor << " ¦ "
                         << setw(14) << temp.estado << " ¦ "
                         << setw(9) << temp.motivo << "¦" << endl;
                    break;
                }
            }
            archivo.close();
        }
    }
    
    cout << "+------------------------------------------------------------------------------+" << endl;
}

//  FUNCIÓN: Cancelar cita
bool cancelarCita(int citaID) {
    int indice = buscarIndiceCitaPorID(citaID);
    if (indice == -1) {
        mostrarError("Cita no encontrada");
        return false;
    }
    
    fstream archivo(ARCHIVO_CITAS, ios::binary | ios::in | ios::out);
    if (!archivo.is_open()) {
        return false;
    }
    
    // Leer cita actual
    long posicion = calcularPosicion<Cita>(indice);
    archivo.seekg(posicion);
    Cita cita;
    archivo.read((char*)&cita, sizeof(Cita));
    
    // Actualizar estado
    strcpy(cita.estado, "Cancelada");
    cita.fechaModificacion = time(0);
    
    // Sobrescribir
    archivo.seekp(posicion);
    archivo.write((char*)&cita, sizeof(Cita));
    archivo.close();
    
    mostrarExito("Cita cancelada correctamente");
    return true;
}

// ============================================================================
// SISTEMA DE HISTORIAL MÉDICO ENLAZADO (FUNCIONALIDAD COMPLETA)
// ============================================================================

//  FUNCIÓN: Agregar consulta al historial (LISTA ENLAZADA EN DISCO)
bool agregarConsultaAlHistorial(HistorialMedico nuevaConsulta) {
    fstream archivo(ARCHIVO_HISTORIALES, ios::binary | ios::in | ios::out);
    if (!archivo.is_open()) {
        mostrarError("No se pudo abrir archivo de historiales");
        return false;
    }
    
    ArchivoHeader header;
    archivo.read((char*)&header, sizeof(ArchivoHeader));
    
    nuevaConsulta.id = header.proximoID;
    nuevaConsulta.fechaRegistro = time(0);
    nuevaConsulta.eliminado = false;
    
    // Obtener paciente
    Paciente paciente = buscarPacientePorID(nuevaConsulta.pacienteID);
    if (paciente.id == -1) {
        archivo.close();
        mostrarError("Paciente no encontrado");
        return false;
    }
    
    // Manejar la lista enlazada
    if (paciente.primerConsultaID == -1) {
        // Primera consulta del paciente
        nuevaConsulta.siguienteConsultaID = -1;
        paciente.primerConsultaID = nuevaConsulta.id;
    } else {
        // Buscar última consulta del paciente
        int consultaActualID = paciente.primerConsultaID;
        int ultimaConsultaID = -1;
        
        HistorialMedico temp;
        while (consultaActualID != -1) {
            // Buscar consulta actual en archivo
            ifstream archivoTemp(ARCHIVO_HISTORIALES, ios::binary);
            ArchivoHeader headerTemp = leerHeader(ARCHIVO_HISTORIALES);
            
            for (int i = 0; i < headerTemp.cantidadRegistros; i++) {
                long posTemp = calcularPosicion<HistorialMedico>(i);
                archivoTemp.seekg(posTemp);
                archivoTemp.read((char*)&temp, sizeof(HistorialMedico));
                
                if (temp.id == consultaActualID && !temp.eliminado) {
                    ultimaConsultaID = consultaActualID;
                    consultaActualID = temp.siguienteConsultaID;
                    break;
                }
            }
            archivoTemp.close();
        }
        
        // Actualizar última consulta
        if (ultimaConsultaID != -1) {
            fstream archivoTemp(ARCHIVO_HISTORIALES, ios::binary | ios::in | ios::out);
            ArchivoHeader headerTemp = leerHeader(ARCHIVO_HISTORIALES);
            
            for (int i = 0; i < headerTemp.cantidadRegistros; i++) {
                long posTemp = calcularPosicion<HistorialMedico>(i);
                archivoTemp.seekg(posTemp);
                archivoTemp.read((char*)&temp, sizeof(HistorialMedico));
                
                if (temp.id == ultimaConsultaID && !temp.eliminado) {
                    temp.siguienteConsultaID = nuevaConsulta.id;
                    archivoTemp.seekp(posTemp);
                    archivoTemp.write((char*)&temp, sizeof(HistorialMedico));
                    break;
                }
            }
            archivoTemp.close();
        }
        
        nuevaConsulta.siguienteConsultaID = -1;
    }
    
    // Escribir nueva consulta
    long posicion = calcularPosicion<HistorialMedico>(header.cantidadRegistros);
    archivo.seekp(posicion);
    archivo.write((char*)&nuevaConsulta, sizeof(HistorialMedico));
    
    // Actualizar header
    header.cantidadRegistros++;
    header.proximoID++;
    header.registrosActivos++;
    
    archivo.seekp(0);
    archivo.write((char*)&header, sizeof(ArchivoHeader));
    archivo.close();
    
    // Actualizar paciente
    paciente.cantidadConsultas++;
    paciente.fechaModificacion = time(0);
    actualizarPaciente(paciente);
    
    // Actualizar hospital global
    hospitalGlobal.siguienteIDConsulta = header.proximoID;
    hospitalGlobal.totalConsultasRealizadas = header.registrosActivos;
    
    cout << "* Consulta agregada al historial. ID: " << nuevaConsulta.id << endl;
    return true;
}

//  FUNCIÓN: Mostrar historial completo de un paciente
void mostrarHistorialMedico(int pacienteID) {
    Paciente paciente = buscarPacientePorID(pacienteID);
    if (paciente.id == -1) {
        mostrarError("Paciente no encontrado");
        return;
    }
    
    if (paciente.cantidadConsultas == 0) {
        cout << "* El paciente no tiene consultas en su historial." << endl;
        return;
    }
    
    cout << "\n+------------------------------------------------------------------------------+" << endl;
    cout << "¦                       HISTORIAL MÉDICO - " << paciente.nombre << " " << paciente.apellido;
    int espacios = 50 - strlen(paciente.nombre) - strlen(paciente.apellido);
    for (int i = 0; i < espacios; i++) cout << " ";
    cout << "¦" << endl;
    cout << "¦----------------------------------------------------------------------------¦" << endl;
    cout << "¦ CONSUL ¦ FECHA      ¦ HORA   ¦ DIAGNoSTICO              ¦ COSTO           ¦" << endl;
    cout << "¦--------+------------+--------+--------------------------+------------------¦" << endl;
    
    // Recorrer lista enlazada
    int consultaActualID = paciente.primerConsultaID;
    int contador = 0;
    
    while (consultaActualID != -1 && contador < 100) { // Límite por seguridad
        // Buscar consulta en archivo
        ifstream archivo(ARCHIVO_HISTORIALES, ios::binary);
        ArchivoHeader header = leerHeader(ARCHIVO_HISTORIALES);
        
        HistorialMedico temp;
        bool encontrada = false;
        
        for (int i = 0; i < header.cantidadRegistros; i++) {
            long posicion = calcularPosicion<HistorialMedico>(i);
            archivo.seekg(posicion);
            archivo.read((char*)&temp, sizeof(HistorialMedico));
            
            if (temp.id == consultaActualID && !temp.eliminado) {
                // Mostrar consulta
                cout << "¦ " << setw(6) << temp.id << " ¦ "
                     << setw(10) << temp.fecha << " ¦ "
                     << setw(6) << temp.hora << " ¦ "
                     << setw(24) << left << temp.diagnostico << " ¦ "
                     << setw(14) << fixed << setprecision(2) << temp.costo << " ¦" << endl;
                
                consultaActualID = temp.siguienteConsultaID;
                encontrada = true;
                contador++;
                break;
            }
        }
        archivo.close();
        
        if (!encontrada) {
            break; // Error en la lista enlazada
        }
    }
    
    cout << "+----------------------------------------------------------------------------+" << endl;
    cout << "Total de consultas: " << paciente.cantidadConsultas << endl;
}

// ============================================================================
// SISTEMA DE COMPACTACIÓN DE ARCHIVOS
// ============================================================================

//  FUNCIÓN: Compactar archivo de pacientes
bool compactarArchivoPacientes() {
    cout << "** Iniciando compactación de archivo de pacientes..." << endl;
    
    // Crear archivo temporal
    const char* archivoTemp = "pacientes_temp.bin";
    fstream temp(archivoTemp, ios::binary | ios::out);
    if (!temp.is_open()) {
        mostrarError("No se pudo crear archivo temporal");
        return false;
    }
    
    // Leer archivo original
    ifstream original(ARCHIVO_PACIENTES, ios::binary);
    if (!original.is_open()) {
        temp.close();
        remove(archivoTemp);
        mostrarError("No se pudo abrir archivo original");
        return false;
    }
    
    ArchivoHeader headerOrig = leerHeader(ARCHIVO_PACIENTES);
    ArchivoHeader headerNuevo;
    headerNuevo.cantidadRegistros = 0;
    headerNuevo.proximoID = headerOrig.proximoID;
    headerNuevo.registrosActivos = 0;
    headerNuevo.version = VERSION_ACTUAL;
    
    // Escribir header nuevo
    temp.write((char*)&headerNuevo, sizeof(ArchivoHeader));
    
    // Copiar solo registros no eliminados
    Paciente p;
    int nuevosIndices[1000] = {0}; // Mapeo de índices viejos a nuevos
    int nuevoContador = 0;
    
    for (int i = 0; i < headerOrig.cantidadRegistros; i++) {
        long posicion = calcularPosicion<Paciente>(i);
        original.seekg(posicion);
        original.read((char*)&p, sizeof(Paciente));
        
        if (!p.eliminado) {
            temp.write((char*)&p, sizeof(Paciente));
            nuevosIndices[i] = nuevoContador;
            nuevoContador++;
            headerNuevo.cantidadRegistros++;
            headerNuevo.registrosActivos++;
        } else {
            nuevosIndices[i] = -1;
        }
    }
    
    original.close();
    
    // Actualizar header del archivo temporal
    temp.seekp(0);
    temp.write((char*)&headerNuevo, sizeof(ArchivoHeader));
    temp.close();
    
    // Reemplazar archivo original
    if (remove(ARCHIVO_PACIENTES) != 0) {
        mostrarError("No se pudo eliminar archivo original");
        remove(archivoTemp);
        return false;
    }
    
    if (rename(archivoTemp, ARCHIVO_PACIENTES) != 0) {
        mostrarError("No se pudo renombrar archivo temporal");
        return false;
    }
    
    cout << "* Compactación completada. " << endl;
    cout << "   Registros antes: " << headerOrig.cantidadRegistros << " (" << headerOrig.registrosActivos << " activos)" << endl;
    cout << "   Registros despues: " << headerNuevo.cantidadRegistros << " (" << headerNuevo.registrosActivos << " activos)" << endl;
    cout << "   Espacio liberado: " << (headerOrig.cantidadRegistros - headerNuevo.cantidadRegistros) << " registros" << endl;
    
    return true;
}

// ============================================================================
// SISTEMA DE RESPALDO Y RESTAURACIÓN
// ============================================================================

//  FUNCIÓN: Crear respaldo completo del sistema
bool crearRespaldo() {
    cout << "** Creando respaldo del sistema..." << endl;
    
    // Crear archivo de respaldo
    ofstream respaldo(RESPALDO_HOSPITAL, ios::binary);
    if (!respaldo.is_open()) {
        mostrarError("No se pudo crear archivo de respaldo");
        return false;
    }
    
    // Escribir marca de tiempo
    time_t ahora = time(0);
    respaldo.write((char*)&ahora, sizeof(time_t));
    
    // Función para copiar archivo
    auto copiarArchivo = [](const char* origen, ostream& destino) {
        ifstream archivo(origen, ios::binary);
        if (!archivo.is_open()) {
            return false;
        }
        
        // Escribir longitud del nombre
        int largoNombre = strlen(origen);
        destino.write((char*)&largoNombre, sizeof(int));
        
        // Escribir nombre
        destino.write(origen, largoNombre);
        
        // Escribir tamaño del archivo
        archivo.seekg(0, ios::end);
        long tamano = archivo.tellg();
        archivo.seekg(0, ios::beg);
        destino.write((char*)&tamano, sizeof(long));
        
        // Copiar contenido
        char buffer[4096];
        while (tamano > 0) {
            int leer = (tamano < 4096) ? tamano : 4096;
            archivo.read(buffer, leer);
            destino.write(buffer, leer);
            tamano -= leer;
        }
        
        archivo.close();
        return true;
    };
    
    // Copiar todos los archivos
    const char* archivos[] = {
        ARCHIVO_HOSPITAL, ARCHIVO_PACIENTES, ARCHIVO_DOCTORES, 
        ARCHIVO_CITAS, ARCHIVO_HISTORIALES
    };
    
    int archivosCopiados = 0;
    for (int i = 0; i < 5; i++) {
        if (copiarArchivo(archivos[i], respaldo)) {
            archivosCopiados++;
            cout << "   * " << archivos[i] << " respaldado" << endl;
        } else {
            cout << "   * " << archivos[i] << " no se pudo respaldar" << endl;
        }
    }
    
    respaldo.close();
    
    if (archivosCopiados == 5) {
        cout << "* Respaldo completado correctamente: " << RESPALDO_HOSPITAL << endl;
        return true;
    } else {
        cout << "**  Respaldo parcial: " << archivosCopiados << "/5 archivos respaldados" << endl;
        return false;
    }
}

//  FUNCIÓN: Restaurar sistema desde respaldo
bool restaurarRespaldo() {
    cout << "** Restaurando sistema desde respaldo..." << endl;
    
    ifstream respaldo(RESPALDO_HOSPITAL, ios::binary);
    if (!respaldo.is_open()) {
        mostrarError("No se encontro archivo de respaldo");
        return false;
    }
    
    // Leer marca de tiempo
    time_t timestamp;
    respaldo.read((char*)&timestamp, sizeof(time_t));
    
    cout << "* Respaldo creado: " << ctime(&timestamp);
    
    // Función para restaurar archivo
    auto restaurarArchivo = [](istream& origen) {
        // Leer longitud del nombre
        int largoNombre;
        origen.read((char*)&largoNombre, sizeof(int));
        
        // Leer nombre
        char* nombreArchivo = new char[largoNombre + 1];
        origen.read(nombreArchivo, largoNombre);
        nombreArchivo[largoNombre] = '\0';
        
        // Leer tamaño
        long tamano;
        origen.read((char*)&tamano, sizeof(long));
        
        // Crear archivo
        ofstream archivo(nombreArchivo, ios::binary);
        if (!archivo.is_open()) {
            delete[] nombreArchivo;
            return false;
        }
        
        // Copiar contenido
        char buffer[4096];
        while (tamano > 0) {
            int leer = (tamano < 4096) ? tamano : 4096;
            origen.read(buffer, leer);
            archivo.write(buffer, leer);
            tamano -= leer;
        }
        
        archivo.close();
        cout << "   * " << nombreArchivo << " restaurado" << endl;
        delete[] nombreArchivo;
        return true;
    };
    
    // Restaurar todos los archivos
    int archivosRestaurados = 0;
    for (int i = 0; i < 5; i++) {
        if (restaurarArchivo(respaldo)) {
            archivosRestaurados++;
        } else {
            cout << "   * Error restaurando archivo " << (i+1) << endl;
        }
    }
    
    respaldo.close();
    
    if (archivosRestaurados == 5) {
        cout << "* Restauracion completada correctamente" << endl;
        
        // Recargar datos del hospital
        return cargarDatosHospital();
    } else {
        cout << "* Restauracion fallida: " << archivosRestaurados << "/5 archivos restaurados" << endl;
        return false;
    }
}

// ============================================================================
// FUNCIONES DE INTERACCIÓN CON EL USUARIO
// ============================================================================

//  FUNCIÓN: Crear nuevo paciente
Paciente crearPacienteInteractivo() {
    Paciente nuevoPaciente;
    
    cout << "\n=== REGISTRO DE NUEVO PACIENTE ===" << endl;
    
    cout << "Nombre: ";
    cin.getline(nuevoPaciente.nombre, 50);
    
    cout << "Apellido: ";
    cin.getline(nuevoPaciente.apellido, 50);
    
    // Validar cédula única
    char cedula[20];
    do {
        cout << "Cedula: ";
        cin.getline(cedula, 20);
        Paciente existente = buscarPacientePorCedula(cedula);
        if (existente.id != -1) {
            cout << "? Error: Ya existe un paciente con esta cedula." << endl;
        } else {
            strcpy(nuevoPaciente.cedula, cedula);
            break;
        }
    } while (true);
    
    cout << "Edad: ";
    cin >> nuevoPaciente.edad;
    limpiarBuffer();
    
    cout << "Sexo (M/F): ";
    cin >> nuevoPaciente.sexo;
    limpiarBuffer();
    
    cout << "Tipo de sangre: ";
    cin.getline(nuevoPaciente.tipoSangre, 5);
    
    cout << "Teléfono: ";
    cin.getline(nuevoPaciente.telefono, 15);
    
    cout << "Dirección: ";
    cin.getline(nuevoPaciente.direccion, 100);
    
    cout << "Email: ";
    cin.getline(nuevoPaciente.email, 50);
    
    cout << "Alergias: ";
    cin.getline(nuevoPaciente.alergias, 500);
    
    cout << "Observaciones: ";
    cin.getline(nuevoPaciente.observaciones, 500);
    
    // Inicializar arrays fijos y contadores
    nuevoPaciente.activo = true;
    nuevoPaciente.cantidadConsultas = 0;
    nuevoPaciente.primerConsultaID = -1;
    nuevoPaciente.cantidadCitas = 0;
    
    return nuevoPaciente;
}

//  FUNCIÓN: Crear nuevo doctor
Doctor crearDoctorInteractivo() {
    Doctor nuevoDoctor;
    
    cout << "\n=== REGISTRO DE NUEVO DOCTOR ===" << endl;
    
    cout << "Nombre: ";
    cin.getline(nuevoDoctor.nombre, 50);
    
    cout << "Apellido: ";
    cin.getline(nuevoDoctor.apellido, 50);
    
    cout << "Cedula profesional: ";
    cin.getline(nuevoDoctor.cedulaProfesional, 20);
    
    cout << "Especialidad: ";
    cin.getline(nuevoDoctor.especialidad, 50);
    
    cout << "Años de experiencia: ";
    cin >> nuevoDoctor.aniosExperiencia;
    limpiarBuffer();
    
    cout << "Costo de consulta: ";
    cin >> nuevoDoctor.costoConsulta;
    limpiarBuffer();
    
    cout << "Horario de atencion: ";
    cin.getline(nuevoDoctor.horarioAtencion, 50);
    
    cout << "Teléfono: ";
    cin.getline(nuevoDoctor.telefono, 15);
    
    cout << "Email: ";
    cin.getline(nuevoDoctor.email, 50);
    
    nuevoDoctor.disponible = true;
    
    return nuevoDoctor;
}

//  FUNCIÓN: Agendar nueva cita
Cita agendarCitaInteractivo() {
    Cita nuevaCita;
    
    cout << "\n=== AGENDAR NUEVA CITA ===" << endl;
    
    // Mostrar pacientes disponibles
    listarPacientes();
    cout << "ID del paciente: ";
    cin >> nuevaCita.pacienteID;
    limpiarBuffer();
    
    Paciente paciente = buscarPacientePorID(nuevaCita.pacienteID);
    if (paciente.id == -1) {
        mostrarError("Paciente no encontrado");
        nuevaCita.id = -1;
        return nuevaCita;
    }
    
    // Mostrar doctores disponibles
    listarDoctores();
    cout << "ID del doctor: ";
    cin >> nuevaCita.doctorID;
    limpiarBuffer();
    
    Doctor doctor = buscarDoctorPorID(nuevaCita.doctorID);
    if (doctor.id == -1) {
        mostrarError("Doctor no encontrado");
        nuevaCita.id = -1;
        return nuevaCita;
    }
    
    // Solicitar fecha y hora
    char fecha[11], hora[6];
    do {
        cout << "Fecha (YYYY-MM-DD): ";
        cin.getline(fecha, 11);
        if (!validarFecha(fecha)) {
            cout << "* Formato de fecha inválido." << endl;
        } else {
            strcpy(nuevaCita.fecha, fecha);
            break;
        }
    } while (true);
    
    do {
        cout << "Hora (HH:MM): ";
        cin.getline(hora, 6);
        if (!validarHora(hora)) {
            cout << "* Formato de hora inválido." << endl;
        } else if (!verificarDisponibilidad(nuevaCita.doctorID, fecha, hora)) {
            cout << "* Doctor no disponible en ese horario." << endl;
        } else {
            strcpy(nuevaCita.hora, hora);
            break;
        }
    } while (true);
    
    cout << "Motivo de la consulta: ";
    cin.getline(nuevaCita.motivo, 150);
    
    // Configurar cita
    strcpy(nuevaCita.estado, "Agendada");
    strcpy(nuevaCita.observaciones, "");
    nuevaCita.atendida = false;
    
    return nuevaCita;
}

//  FUNCIÓN: Agregar consulta al historial
HistorialMedico crearConsultaInteractivo() {
    HistorialMedico nuevaConsulta;
    
    cout << "\n=== REGISTRAR CONSULTA MEDICA ===" << endl;
    
    cout << "ID del paciente: ";
    cin >> nuevaConsulta.pacienteID;
    limpiarBuffer();
    
    Paciente paciente = buscarPacientePorID(nuevaConsulta.pacienteID);
    if (paciente.id == -1) {
        mostrarError("Paciente no encontrado");
        nuevaConsulta.id = -1;
        return nuevaConsulta;
    }
    
    cout << "ID del doctor: ";
    cin >> nuevaConsulta.doctorID;
    limpiarBuffer();
    
    Doctor doctor = buscarDoctorPorID(nuevaConsulta.doctorID);
    if (doctor.id == -1) {
        mostrarError("Doctor no encontrado");
        nuevaConsulta.id = -1;
        return nuevaConsulta;
    }
    
    cout << "Fecha (YYYY-MM-DD): ";
    cin.getline(nuevaConsulta.fecha, 11);
    
    cout << "Hora (HH:MM): ";
    cin.getline(nuevaConsulta.hora, 6);
    
    cout << "Diagnostico: ";
    cin.getline(nuevaConsulta.diagnostico, 200);
    
    cout << "Tratamiento: ";
    cin.getline(nuevaConsulta.tratamiento, 200);
    
    cout << "Medicamentos: ";
    cin.getline(nuevaConsulta.medicamentos, 150);
    
    cout << "Costo: ";
    cin >> nuevaConsulta.costo;
    limpiarBuffer();
    
    return nuevaConsulta;
}

// ============================================================================
// FUNCIONES DE REPORTES Y ESTADÍSTICAS
// ============================================================================

//  FUNCIÓN: Mostrar estadísticas del sistema
void mostrarEstadisticas() {
    ArchivoHeader h_pacientes = leerHeader(ARCHIVO_PACIENTES);
    ArchivoHeader h_doctores = leerHeader(ARCHIVO_DOCTORES);
    ArchivoHeader h_citas = leerHeader(ARCHIVO_CITAS);
    ArchivoHeader h_historiales = leerHeader(ARCHIVO_HISTORIALES);
    
    
    cout << "\n** ESTADISTICAS DEL SISTEMA" << endl;
    cout << "============================" << endl;
    
    cout << endl;
    
    cout << "** PACIENTES:" << endl;
    cout << "   Registrados: " << hospitalGlobal.totalPacientesRegistrados << endl;
    cout << "   En archivo: " << h_pacientes.registrosActivos << " activos de " << h_pacientes.cantidadRegistros << " totales" << endl;
    cout << "   Próximo ID: " << hospitalGlobal.siguienteIDPaciente << endl;
    cout << endl;
    
    cout << "** DOCTORES:" << endl;
    cout << "   Registrados: " << hospitalGlobal.totalDoctoresRegistrados << endl;
    cout << "   En archivo: " << h_doctores.registrosActivos << " activos de " << h_doctores.cantidadRegistros << " totales" << endl;
    cout << "   Próximo ID: " << hospitalGlobal.siguienteIDDoctor << endl;
    cout << endl;
    
    cout << "** CITAS:" << endl;
    cout << "   Agendadas: " << hospitalGlobal.totalCitasAgendadas << endl;
    cout << "   En archivo: " << h_citas.registrosActivos << " activas de " << h_citas.cantidadRegistros << " totales" << endl;
    cout << "   Proximo ID: " << hospitalGlobal.siguienteIDCita << endl;
    cout << endl;
    
    cout << "** CONSULTAS:" << endl;
    cout << "   Realizadas: " << hospitalGlobal.totalConsultasRealizadas << endl;
    cout << "   En archivo: " << h_historiales.registrosActivos << " activas de " << h_historiales.cantidadRegistros << " totales" << endl;
    cout << "   Próximo ID: " << hospitalGlobal.siguienteIDConsulta << endl;
}


#endif //FUNCIONES_H
