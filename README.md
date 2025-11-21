*Diagrama de Relaciones de Archivos*
  
  hospital.bin        pacientes.bin         doctores.bin   
                                                         
- Configuración ◄──► - Datos pacientes◄──► - Datos doctores
- Contadores ID      - Arrays fijos        - Arrays fijos   
                                     
                           ▼                      ▼
                        citas.bin          historiales.bin 
                        - Relaciones         - Lista enlazada
                        - Estados            - Consultas     


*Formato de Estructuras*

  struct ArchivoHeader {
    int cantidadRegistros;      
    int proximoID;              
    int registrosActivos;      
    int version;                
};

struct Hospital {
    char nombre[100];           
    char direccion[150];         
    char telefono[15];          
    
  int siguienteIDPaciente;    
  int siguienteIDDoctor;      
  int siguienteIDCita;        
  int siguienteIDConsulta;    
    
   int totalPacientesRegistrados;  
   int totalDoctoresRegistrados;   
   int totalCitasAgendadas;        
   int totalConsultasRealizadas;   
};

struct Paciente {
  
  int id;                     
  char nombre[50];            
  char apellido[50];          
  char cedula[20];            
  int edad;                   
  char sexo;                  
  char tipoSangre[5];         
  char telefono[15];          
  char direccion[100];        
  char email[50];             
  char alergias[500];         
  char observaciones[500];    
    
  bool activo;                
  int cantidadConsultas;      
  int primerConsultaID;       
  int cantidadCitas;          
  int citasIDs[20];           
    
   
  bool eliminado;             
  time_t fechaCreacion;       
  time_t fechaModificacion;   
};

struct HistorialMedico {

  int id;                     
  int pacienteID;            
  char fecha[11];             
  char hora[6];               
  char diagnostico[200];      
  char tratamiento[200];      
  char medicamentos[150];     
  int doctorID;               
  float costo;                
    
    
  int siguienteConsultaID;    
    
   
  bool eliminado;            
  time_t fechaRegistro;       
};

struct Cita {
    // Datos básicos (391 bytes)
    int id;                     
    int pacienteID;            
    int doctorID;               
    char fecha[11];             
    char hora[6];               
    char motivo[150];           
    char estado[20];            
    char observaciones[200];    
    
    
  bool atendida;              
  int consultaID;             
    
    
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
  int cantidadPacientes;     
  int pacientesIDs[50];      
  int cantidadCitas;          
  int citasIDs[30];           
    
    
  bool eliminado;            
  time_t fechaCreacion;       
  time_t fechaModificacion;  
};

*Sistema de Archivos - Funciones Fundamentales*

bool inicializarArchivo(const char* nombreArchivo)
Propósito: Crear un nuevo archivo binario con header inicializado

ArchivoHeader leerHeader(const char* nombreArchivo)
Propósito: Leer el header de cualquier archivo del sistema

bool actualizarHeader(const char* nombreArchivo, ArchivoHeader nuevoHeader)
Propósito: Actualizar el header de un archivo existente

template<typename T> long calcularPosicion(int indice)
Propósito: Calcular posición en bytes para acceso aleatorio

bool agregarPaciente(Paciente nuevoPaciente)
Propósito: Agregar nuevo paciente al archivo con ID auto-incremento

Paciente buscarPacientePorID(int id)
Propósito: Buscar paciente por ID usando acceso aleatorio

Paciente buscarPacientePorCedula(const char* cedula)
Propósito: Buscar paciente por cédula (búsqueda secuencial)

