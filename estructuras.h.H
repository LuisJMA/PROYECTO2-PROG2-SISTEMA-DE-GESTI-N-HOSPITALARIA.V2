#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H
#include <ctime>


// ============================================================================
// CONSTANTES Y CONFIGURACIÓN
// ============================================================================

const char* ARCHIVO_HOSPITAL = "hospital.bin";
const char* ARCHIVO_PACIENTES = "pacientes.bin";
const char* ARCHIVO_DOCTORES = "doctores.bin";
const char* ARCHIVO_CITAS = "citas.bin";
const char* ARCHIVO_HISTORIALES = "historiales.bin";
const char* RESPALDO_HOSPITAL = "respaldo_hospital.bak";

const int VERSION_ACTUAL = 1;
const int MAX_CITAS_PACIENTE = 20;
const int MAX_CITAS_DOCTOR = 30;
const int MAX_PACIENTES_DOCTOR = 50;

// ============================================================================
// ESTRUCTURAS DE DATOS
// ============================================================================

struct ArchivoHeader {
    int cantidadRegistros;      // Cantidad actual de registros
    int proximoID;              // Siguiente ID disponible  
    int registrosActivos;       // Registros no eliminados
    int version;                // Versión del formato
};

struct HistorialMedico {
    int id;
    int pacienteID;                 // Referencia al paciente
    char fecha[11];                 // YYYY-MM-DD
    char hora[6];                   // HH:MM
    char diagnostico[200];
    char tratamiento[200];
    char medicamentos[150];
    int doctorID;
    float costo;
    
    // Navegación enlazada - LISTA ENLAZADA EN DISCO
    int siguienteConsultaID;        // ID de siguiente consulta (-1 si es última)
    
    // Metadata
    bool eliminado;
    time_t fechaRegistro;
};

struct Paciente {
    int id;
    char nombre[50];
    char apellido[50];
    char cedula[20];
    int edad;
    char sexo;                      // 'M' o 'F'
    char tipoSangre[5];
    char telefono[15];
    char direccion[100];
    char email[50];
    char alergias[500];
    char observaciones[500];
    bool activo;
    
    // Índices para relaciones (reemplazan arrays dinámicos)
    int cantidadConsultas;          // Total de consultas en historial
    int primerConsultaID;           // ID de primera consulta (-1 si no tiene)
    
    int cantidadCitas;              // Total de citas agendadas
    int citasIDs[MAX_CITAS_PACIENTE]; // Array FIJO de IDs de citas
    
    // Metadata de registro
    bool eliminado;
    time_t fechaCreacion;
    time_t fechaModificacion;
};

struct Doctor {
    int id;
    char nombre[50];
    char apellido[50];
    char cedulaProfesional[20];
    char especialidad[50];
    int aniosExperiencia;
    float costoConsulta;
    char horarioAtencion[50];
    char telefono[15];
    char email[50];
    bool disponible;
    
    // Relaciones con arrays fijos
    int cantidadPacientes;
    int pacientesIDs[MAX_PACIENTES_DOCTOR]; // Máximo 50 pacientes
    
    int cantidadCitas;
    int citasIDs[MAX_CITAS_DOCTOR];         // Máximo 30 citas
    
    // Metadata
    bool eliminado;
    time_t fechaCreacion;
    time_t fechaModificacion;
};

struct Cita {
    int id;
    int pacienteID;
    int doctorID;
    char fecha[11];                 // YYYY-MM-DD
    char hora[6];                   // HH:MM
    char motivo[150];
    char estado[20];                // "Agendada", "Atendida", "Cancelada"
    char observaciones[200];
    bool atendida;
    
    // Referencia al historial
    int consultaID;                 // ID de consulta (-1 si no atendida)
    
    // Metadata
    bool eliminado;
    time_t fechaCreacion;
    time_t fechaModificacion;
};

struct Hospital {
    // SOLO datos básicos - NO arrays dinámicos
    char nombre[100];
    char direccion[150];
    char telefono[15];
    
    // Contadores de IDs (auto-increment)
    int siguienteIDPaciente;
    int siguienteIDDoctor;
    int siguienteIDCita;
    int siguienteIDConsulta;
    
    // Estadísticas generales
    int totalPacientesRegistrados;
    int totalDoctoresRegistrados;
    int totalCitasAgendadas;
    int totalConsultasRealizadas;
};

// ============================================================================
// VARIABLES GLOBALES
// ============================================================================

Hospital hospitalGlobal;
#endif //ESTRUCTURAS_H
