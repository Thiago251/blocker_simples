#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_SITES 100
#define MAX_URL_LENGTH 256

DWORD WINAPI show_popup_thread(LPVOID param);

char blocked_sites[MAX_SITES][MAX_URL_LENGTH];
int site_count = 0;
time_t block_start_time = 0;
int is_blocked = 0;

// Carrega lista de sites bloqueados
void load_blocked_sites() {
    FILE *file = fopen("sites_bloqueados.txt", "r");
    if (file == NULL) {
        // Cria arquivo padrão se não existir
        file = fopen("sites_bloqueados.txt", "w");
        if (file != NULL) {
            fprintf(file, "pornhub.com\n");
            fprintf(file, "xvideos.com\n");
            fprintf(file, "redtube.com\n");
            fprintf(file, "xhamster.com\n");
            fprintf(file, "porn\n");
            fprintf(file, "sex\n");
            fprintf(file, "xxx\n");
            fclose(file);
        }
        file = fopen("sites_bloqueados.txt", "r");
    }
    
    if (file != NULL) {
        char line[MAX_URL_LENGTH];
        site_count = 0;
        while (fgets(line, sizeof(line), file) && site_count < MAX_SITES) {
            // Remove quebra de linha
            line[strcspn(line, "\n")] = 0;
            if (strlen(line) > 0) {
                strcpy(blocked_sites[site_count], line);
                site_count++;
            }
        }
        fclose(file);
    }
}

// Verifica se o texto contém site bloqueado
int contains_blocked_content(const char* text) {
    int i;
    for (i = 0; i < site_count; i++) {
        if (strstr(text, blocked_sites[i]) != NULL) {
            return 1;
        }
    }
    return 0;
}

// Fecha todos os navegadores
void close_browsers() {
    system("taskkill /f /im chrome.exe >nul 2>&1");
    system("taskkill /f /im firefox.exe >nul 2>&1");
    system("taskkill /f /im msedge.exe >nul 2>&1");
    system("taskkill /f /im iexplore.exe >nul 2>&1");
    system("taskkill /f /im opera.exe >nul 2>&1");
}

// Mostra popup de bloqueio
void show_block_message() {
    if (!is_blocked) {
        is_blocked = 1;
        block_start_time = time(NULL);
        close_browsers();
        
        // Cria thread para mostrar popup
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)show_popup_thread, NULL, 0, NULL);
    }
}

// Thread do popup que não pode ser fechado por 5 minutos
DWORD WINAPI show_popup_thread(LPVOID param) {
    time_t current_time;
    int remaining_seconds;
    char message[512];
    
    while (1) {
        current_time = time(NULL);
        remaining_seconds = 300 - (int)(current_time - block_start_time); // 5 minutos = 300 segundos
        
        if (remaining_seconds <= 0) {
            is_blocked = 0;
            MessageBox(NULL, 
                "Tempo de bloqueio finalizado!\nVoce pode usar o navegador novamente.", 
                "Por�m estamos de olho - Bloqueio Finalizado", 
                MB_OK | MB_ICONINFORMATION);
            break;
        }
        
        sprintf(message, 
            "Voce nao esta pronto para matrix Neo.\nA matrix pegou voce!\n\n"
            "Site de pornografia bloqueado!\n\n"
            "Tempo restante: %d minutos e %d segundos\n\n"
            "Estamos de olhos em voce agora Ass: Agente Smith .",
            remaining_seconds / 60, remaining_seconds % 60);
        
        // Mostra popup que não pode ser fechado
        int result = MessageBox(NULL, message, "MATRIX - ACESSO NEGADO", 
                               MB_OK | MB_ICONERROR | MB_SYSTEMMODAL | MB_TOPMOST);
        
        Sleep(10000); // Espera 10 segundos antes de mostrar novamente
    }
    
    return 0;
}

// Hook do teclado para capturar digitação
LRESULT CALLBACK keyboard_hook(int nCode, WPARAM wParam, LPARAM lParam) {
    static char typed_text[1024] = "";
    static int text_pos = 0;
    
    if (nCode >= 0 && wParam == WM_KEYDOWN) {
        KBDLLHOOKSTRUCT* kbd = (KBDLLHOOKSTRUCT*)lParam;
        
        // Se já está bloqueado, impede navegadores de abrir
        if (is_blocked) {
            if (kbd->vkCode == VK_RETURN || kbd->vkCode == VK_SPACE) {
                close_browsers();
                return 1; // Bloqueia a tecla
            }
        }
        
        // Captura caracteres digitados
        if (kbd->vkCode >= 'A' && kbd->vkCode <= 'Z') {
            if (text_pos < sizeof(typed_text) - 1) {
                typed_text[text_pos++] = (char)kbd->vkCode + 32; // minúscula
                typed_text[text_pos] = '\0';
            }
        } else if (kbd->vkCode == VK_SPACE || kbd->vkCode == VK_RETURN) {
            // Verifica se o texto digitado contém conteúdo bloqueado
            if (contains_blocked_content(typed_text)) {
                show_block_message();
            }
            
            // Limpa buffer se ficou muito grande
            if (text_pos > 500) {
                text_pos = 0;
                typed_text[0] = '\0';
            }
        } else if (kbd->vkCode == VK_BACK && text_pos > 0) {
            text_pos--;
            typed_text[text_pos] = '\0';
        }
    }
    
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

// Monitora janelas de navegador
DWORD WINAPI monitor_windows(LPVOID param) {
    while (1) {
        if (is_blocked) {
            // Fecha qualquer navegador que tentar abrir durante o bloqueio
            close_browsers();
        }
        
        // Verifica títulos de janelas
        HWND hwnd = GetForegroundWindow();
        if (hwnd != NULL) {
            char window_title[256];
            GetWindowText(hwnd, window_title, sizeof(window_title));
            
            // Converte para minúscula
            int i;
            for (i = 0; window_title[i]; i++) {
                window_title[i] = tolower(window_title[i]);
            }
            
            if (contains_blocked_content(window_title)) {
                show_block_message();
            }
        }
        
        Sleep(1000); // Verifica a cada segundo
    }
    return 0;
}

int main() {
    // Esconde a janela do console
    ShowWindow(GetConsoleWindow(), SW_HIDE);
    
    // Carrega lista de sites bloqueados
    load_blocked_sites();
    
    printf("Matrix Blocker iniciado...\n");
    printf("Sites bloqueados carregados: %d\n", site_count);
    
    // Instala hook do teclado
    HHOOK keyboard_hook_handle = SetWindowsHookEx(WH_KEYBOARD_LL, keyboard_hook, GetModuleHandle(NULL), 0);
    if (keyboard_hook_handle == NULL) {
        MessageBox(NULL, "Erro ao instalar hook do teclado!", "Erro", MB_OK | MB_ICONERROR);
        return 1;
    }
    
    // Inicia thread de monitoramento de janelas
    CreateThread(NULL, 0, monitor_windows, NULL, 0, NULL);
    
    // Loop principal
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    UnhookWindowsHookEx(keyboard_hook_handle);
    return 0;
}
