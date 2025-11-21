#ifndef MENUS_H
#define MENUS_H

#include <iostream>
#include "FUNCIONES.h"

using namespace std;




// SISTEMA DE MENÚS
// ============================================================================

//  FUNCIÓN: Menú de gestión de pacientes
void menuPacientes() {
    int opcion;
    do {
        cout << "\n+----------------------------------------+" << endl;
        cout << "¦          GESTIÓN DE PACIENTES         ¦" << endl;
        cout << "¦----------------------------------------¦" << endl;
        cout << "¦ 1. Registrar nuevo paciente           ¦" << endl;
        cout << "¦ 2. Buscar paciente por cedula         ¦" << endl;
        cout << "¦ 3. Buscar paciente por ID             ¦" << endl;
        cout << "¦ 4. Ver historial medico               ¦" << endl;
        cout << "¦ 5. Ver citas del paciente             ¦" << endl;
        cout << "¦ 6. Listar todos los pacientes         ¦" << endl;
        cout << "¦ 0. Volver al menu principal           ¦" << endl;
        cout << "+----------------------------------------+" << endl;
        cout << "Opcion: ";
        cin >> opcion;
        limpiarBuffer();
        
        switch (opcion) {
            case 1: {
                Paciente nuevo = crearPacienteInteractivo();
                if (agregarPaciente(nuevo)) {
                    mostrarExito("Paciente creado exitosamente");
                }
                break;
            }
            case 2: {
                char cedula[20];
                cout << "Cedula a buscar: ";
                cin.getline(cedula, 20);
                Paciente paciente = buscarPacientePorCedula(cedula);
                if (paciente.id != -1) {
                    cout << "* Paciente encontrado:" << endl;
                    cout << "   ID: " << paciente.id << endl;
                    cout << "   Nombre: " << paciente.nombre << " " << paciente.apellido << endl;
                    cout << "   Cédula: " << paciente.cedula << endl;
                    cout << "   Teléfono: " << paciente.telefono << endl;
                    cout << "   Email: " << paciente.email << endl;
                    cout << "   Consultas: " << paciente.cantidadConsultas << endl;
                    cout << "   Citas: " << paciente.cantidadCitas << endl;
                } else {
                    mostrarError("Paciente no encontrado");
                }
                break;
            }
            case 3: {
                int id;
                cout << "ID del paciente: ";
                cin >> id;
                limpiarBuffer();
                Paciente paciente = buscarPacientePorID(id);
                if (paciente.id != -1) {
                    cout << "* Paciente encontrado:" << endl;
                    cout << "   ID: " << paciente.id << endl;
                    cout << "   Nombre: " << paciente.nombre << " " << paciente.apellido << endl;
                    cout << "   Cédula: " << paciente.cedula << endl;
                    cout << "   Edad: " << paciente.edad << endl;
                    cout << "   Sexo: " << paciente.sexo << endl;
                    cout << "   Tipo sangre: " << paciente.tipoSangre << endl;
                    cout << "   Teléfono: " << paciente.telefono << endl;
                    cout << "   Email: " << paciente.email << endl;
                    cout << "   Alergias: " << paciente.alergias << endl;
                    cout << "   Observaciones: " << paciente.observaciones << endl;
                    cout << "   Consultas: " << paciente.cantidadConsultas << endl;
                    cout << "   Citas: " << paciente.cantidadCitas << endl;
                } else {
                    mostrarError("Paciente no encontrado");
                }
                break;
            }
            case 4: {
                int id;
                cout << "ID del paciente: ";
                cin >> id;
                limpiarBuffer();
                mostrarHistorialMedico(id);
                break;
            }
            case 5: {
                int id;
                cout << "ID del paciente: ";
                cin >> id;
                limpiarBuffer();
                listarCitasPaciente(id);
                break;
            }
            case 6:
                listarPacientes();
                break;
            case 0:
                cout << "Volviendo al menu principal..." << endl;
                break;
            default:
                mostrarError("Opcion invalida");
        }
    } while (opcion != 0);
}

//  FUNCIÓN: Menú de gestión de doctores
void menuDoctores() {
    int opcion;
    do {
        cout << "\n+----------------------------------------+" << endl;
        cout << "¦           GESTION DE DOCTORES          ¦" << endl;
        cout << "¦----------------------------------------¦" << endl;
        cout << "¦ 1. Registrar nuevo doctor             ¦" << endl;
        cout << "¦ 2. Buscar doctor por ID               ¦" << endl;
        cout << "¦ 3. Listar todos los doctores          ¦" << endl;
        cout << "¦ 0. Volver al menu principal           ¦" << endl;
        cout << "+----------------------------------------+" << endl;
        cout << "Opcion: ";
        cin >> opcion;
        limpiarBuffer();
        
        switch (opcion) {
            case 1: {
                Doctor nuevo = crearDoctorInteractivo();
                if (agregarDoctor(nuevo)) {
                    mostrarExito("Doctor creado exitosamente");
                }
                break;
            }
            case 2: {
                int id;
                cout << "ID del doctor: ";
                cin >> id;
                limpiarBuffer();
                Doctor doctor = buscarDoctorPorID(id);
                if (doctor.id != -1) {
                    cout << "* Doctor encontrado:" << endl;
                    cout << "   ID: " << doctor.id << endl;
                    cout << "   Nombre: " << doctor.nombre << " " << doctor.apellido << endl;
                    cout << "   Especialidad: " << doctor.especialidad << endl;
                    cout << "   Cédula: " << doctor.cedulaProfesional << endl;
                    cout << "   Años experiencia: " << doctor.aniosExperiencia << endl;
                    cout << "   Costo consulta: $" << doctor.costoConsulta << endl;
                    cout << "   Horario: " << doctor.horarioAtencion << endl;
                    cout << "   Telefono: " << doctor.telefono << endl;
                    cout << "   Email: " << doctor.email << endl;
                    cout << "   Pacientes: " << doctor.cantidadPacientes << endl;
                    cout << "   Citas: " << doctor.cantidadCitas << endl;
                    cout << "   Disponible: " << (doctor.disponible ? "Si" : "No") << endl;
                } else {
                    mostrarError("Doctor no encontrado");
                }
                break;
            }
            case 3:
                listarDoctores();
                break;
            case 0:
                cout << "Volviendo al menu principal..." << endl;
                break;
            default:
                mostrarError("Opcion invalida");
        }
    } while (opcion != 0);
}

//  FUNCIÓN: Menú de gestión de citas
void menuCitas() {
    int opcion;
    do {
        cout << "\n+----------------------------------------+" << endl;
        cout << "¦            GESTIÓN DE CITAS            ¦" << endl;
        cout << "¦----------------------------------------¦" << endl;
        cout << "¦ 1. Agendar nueva cita                 ¦" << endl;
        cout << "¦ 2. Ver citas de paciente              ¦" << endl;
        cout << "¦ 3. Cancelar cita                      ¦" << endl;
        cout << "¦ 4. Verificar disponibilidad           ¦" << endl;
        cout << "¦ 0. Volver al menu principal           ¦" << endl;
        cout << "+----------------------------------------+" << endl;
        cout << "Opcion: ";
        cin >> opcion;
        limpiarBuffer();
        
        switch (opcion) {
            case 1: {
                Cita nueva = agendarCitaInteractivo();
                if (nueva.id != -1 && agregarCita(nueva)) {
                    mostrarExito("Cita agendada exitosamente");
                }
                break;
            }
            case 2: {
                int id;
                cout << "ID del paciente: ";
                cin >> id;
                limpiarBuffer();
                listarCitasPaciente(id);
                break;
            }
            case 3: {
                int id;
                cout << "ID de la cita a cancelar: ";
                cin >> id;
                limpiarBuffer();
                cancelarCita(id);
                break;
            }
            case 4: {
                int doctorID;
                char fecha[11], hora[6];
                
                cout << "ID del doctor: ";
                cin >> doctorID;
                limpiarBuffer();
                
                cout << "Fecha (YYYY-MM-DD): ";
                cin.getline(fecha, 11);
                
                cout << "Hora (HH:MM): ";
                cin.getline(hora, 6);
                
                if (verificarDisponibilidad(doctorID, fecha, hora)) {
                    cout << "*El doctor esta disponible en ese horario." << endl;
                } else {
                    cout << "*El doctor NO está disponible en ese horario." << endl;
                }
                break;
            }
            case 0:
                cout << "Volviendo al menu principal..." << endl;
                break;
            default:
                mostrarError("Opcion invalida");
        }
    } while (opcion != 0);
}

//  FUNCIÓN: Menú de historial médico
void menuHistorial() {
    int opcion;
    do {
        cout << "\n+----------------------------------------+" << endl;
        cout << "¦         HISTORIAL MÉDICO              ¦" << endl;
        cout << "¦----------------------------------------¦" << endl;
        cout << "¦ 1. Agregar consulta al historial      ¦" << endl;
        cout << "¦ 2. Ver historial de paciente          ¦" << endl;
        cout << "¦ 0. Volver al menú principal           ¦" << endl;
        cout << "+----------------------------------------+" << endl;
        cout << "Opcion: ";
        cin >> opcion;
        limpiarBuffer();
        
        switch (opcion) {
            case 1: {
                HistorialMedico nueva = crearConsultaInteractivo();
                if (nueva.id != -1 && agregarConsultaAlHistorial(nueva)) {
                    mostrarExito("Consulta agregada al historial");
                }
                break;
            }
            case 2: {
                int id;
                cout << "ID del paciente: ";
                cin >> id;
                limpiarBuffer();
                mostrarHistorialMedico(id);
                break;
            }
            case 0:
                cout << "Volviendo al menu principal..." << endl;
                break;
            default:
                mostrarError("Opcion invalida");
        }
    } while (opcion != 0);
}

//  FUNCIÓN: Menú de mantenimiento
void menuMantenimiento() {
    int opcion;
    do {
        cout << "\n+----------------------------------------+" << endl;
        cout << "¦          MANTENIMIENTO                ¦" << endl;
        cout << "¦----------------------------------------¦" << endl;
        cout << "¦ 1. Compactar archivos                 ¦" << endl;
        cout << "¦ 2. Crear respaldo                     ¦" << endl;
        cout << "¦ 3. Restaurar respaldo                 ¦" << endl;
        cout << "¦ 4. Verificar archivos                 ¦" << endl;
        cout << "¦ 0. Volver al menu principal           ¦" << endl;
        cout << "+----------------------------------------+" << endl;
        cout << "Opcion: ";
        cin >> opcion;
        limpiarBuffer();
        
        switch (opcion) {
            case 1:
                compactarArchivoPacientes();
                break;
            case 2:
                crearRespaldo();
                break;
            case 3:
                restaurarRespaldo();
                break;
            case 4:
                cout << "*Verificando archivos del sistema..." << endl;
                verificarArchivo(ARCHIVO_HOSPITAL);
                verificarArchivo(ARCHIVO_PACIENTES);
                verificarArchivo(ARCHIVO_DOCTORES);
                verificarArchivo(ARCHIVO_CITAS);
                verificarArchivo(ARCHIVO_HISTORIALES);
                break;
            case 0:
                cout << "Volviendo al menu principal..." << endl;
                break;
            default:
                mostrarError("Opcion invalida");
        }
    } while (opcion != 0);
}

//  FUNCIÓN: Menú principal
void menuPrincipal() {
    int opcion;
    do {
        cout << "\n+----------------------------------------+" << endl;
        cout << "¦   SISTEMA DE GESTION HOSPITALARIA v2   ¦" << endl;
        cout << "¦----------------------------------------¦" << endl;
        cout << "¦ 1. Gestion de Pacientes               ¦" << endl;
        cout << "¦ 2. Gestion de Doctores                ¦" << endl;
        cout << "¦ 3. Gestion de Citas                   ¦" << endl;
        cout << "¦ 4. Historial Medico                   ¦" << endl;
        cout << "¦ 5. Reportes y Estadisticas            ¦" << endl;
        cout << "¦ 6. Mantenimiento                      ¦" << endl;
        cout << "¦ 7. Guardar y Salir                    ¦" << endl;
        cout << "+----------------------------------------+" << endl;
        cout << "Opcion: ";
        cin >> opcion;
        limpiarBuffer();
        
        switch (opcion) {
            case 1:
                menuPacientes();
                break;
            case 2:
                menuDoctores();
                break;
            case 3:
                menuCitas();
                break;
            case 4:
                menuHistorial();
                break;
            case 5:
                mostrarEstadisticas();
                break;
            case 6:
                menuMantenimiento();
                break;
            case 7:
                cout << "*Guardando datos..." << endl;
                guardarDatosHospital();
                cout << "*Saliendo del sistema..." << endl;
                break;
            default:
                mostrarError("Opcion invalida");
        }
    } while (opcion != 7);
}

#endif//MENUS_H
