#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define tam 10

typedef struct aluno {
    char matricula[12];
    char nome[250];
    char email[100];
    char telefone[12];
    int qnt_disciplinas;
    int status;
} Aluno;

typedef struct disciplina {
    char codigo_disciplina[8];
    char nome_disciplina[250];
    char horario_disciplina[100];
    int sala_aula;
    int qnt_vagasTotal;
    int qnt_vagasOcupadas;
    int status;
} Disciplina;

typedef struct {
    char matricula[12];
    char codigo_disciplina[8];
    char data_matricula[9];  
    int status; 
} Matricula;

void menu() {
    printf(" --- Menu principal ---\n");
    printf("Disciplina - 1\n");
    printf("Aluno - 2\n");
    printf("Matrículas - 3\n");
    printf("Finalizar programa - 4\n");
}
void menu_disciplina() {
    printf(" --- Menu para disciplinas ---\n");
    printf("Cadastrar nova disciplina - 1\n");
    printf("Excluir disciplina existente - 2\n");
    printf("Alterar dados da disciplina - 3\n");
    printf("Exibir dados de uma disciplina - 4\n");
    printf("Retornar ao menu principal - 5\n");
}
void menu_aluno() {
    printf(" --- Menu para alunos ---\n");
    printf("Cadastrar novo aluno - 1\n");
    printf("Alterar dados de um aluno - 2\n");
    printf("Exibir dados de um aluno - 3\n");
    printf("Remover aluno - 4\n");
    printf("Retornar ao menu principal - 5\n");
}
void menu_matricula() {
    printf(" --- Menu para matriculas ---\n");
    printf("Inclusão de matrícula - 1\n");
    printf("Exclusão de matrícula - 2\n");
    printf("Retornar ao menu principal - 3\n");
}

int consulta_aluno(const char* matricula_procurada) {
    int i = 0;
    FILE *file;
    Aluno al;
    
    file = fopen("arquivo_aluno", "rb");
    if (file == NULL) {
        printf("Erro ao tentar abrir o arquivo.\n");
        return -2;  
    }
    
    while (fread(&al, sizeof(Aluno), 1, file)) {
        if (strcmp(al.matricula, matricula_procurada) == 0) {
            fclose(file); 
            return i;  
        }
        i++;
    }

    fclose(file);
    return -1;  
}

int email(char* email) {
    int i;
    int cont_arroba = 0, cont_ponto = 0;

    for (i = 0; email[i] != '\0'; i++) {
        if (email[i] == '@') {
            cont_arroba = 1;
        } else if (email[i] == '.' && cont_arroba) {
            cont_ponto = 1; 
        }
    }
    if (cont_arroba && cont_ponto) {
        return 1; 
    } else {
        return -1; 
    }
}

int nome(char *nome) {
    for (int i = 0; nome[i] != '\0'; i++) {
        if (!isalpha(nome[i]) && !isspace(nome[i])) {
            return 0;  
        }
    }
    return 1; 
}

int telefone(char* telefone){
    int i;
    for(i = 0; telefone[i] != '\0'; i++){
        if(!isdigit(telefone[i])){
            return 0;
        }
    }
    return 1;
}

void cadastrar_aluno() {
    Aluno al;
    FILE *file_cadastroaluno;

    file_cadastroaluno = fopen("arquivo_aluno", "a+b");
    if (file_cadastroaluno == NULL) {
        printf("Erro ao tentar criar/abrir arquivo! \n");
        return;  
    }

    printf("Cadastrando aluno.\n");
    printf("Nome do aluno: ");
    fgets(al.nome, sizeof(al.nome), stdin);
    al.nome[strcspn(al.nome, "\n")] = '\0';  
    if (nome(al.nome) != 1) {
        printf("O nome inserido não é válido! \n");
        fclose(file_cadastroaluno);
        return;
    }

    printf("\nEmail: ");
    fgets(al.email, sizeof(al.email), stdin);
    al.email[strcspn(al.email, "\n")] = '\0';  
    if (email(al.email) != 1) {
        printf("O E-mail não é válido! Tente novamente. \n");
        fclose(file_cadastroaluno);
        return;
    }

    printf("\nTelefone: ");
    fgets(al.telefone, sizeof(al.telefone), stdin);
    al.telefone[strcspn(al.telefone, "\n")] = '\0';  
    if (telefone(al.telefone) != 1) {
        printf("O telefone inserido não é válido! \n");
        fclose(file_cadastroaluno);
        return;
    }
    printf("\nMatrícula: ");
    fgets(al.matricula, sizeof(al.matricula), stdin);
    al.matricula[strcspn(al.matricula, "\n")] = '\0'; 
    if (consulta_aluno(al.matricula) != -1) {
        printf("Esse aluno já está cadastrado! Cadastro não efetivado.\n");
        fclose(file_cadastroaluno);
        return;
    }
    al.qnt_disciplinas = 0;
    al.status = 1; 
    fwrite(&al, sizeof(Aluno), 1, file_cadastroaluno);
    printf("\nAluno cadastrado com sucesso!\n");
    fclose(file_cadastroaluno);
}

void alterar_dados(char matricula[]) {
    int aluno = consulta_aluno(matricula), verificar;
    Aluno al;
    FILE *file;
    if (aluno == -1) {
        printf("A matrícula inserida não existe! Digite uma matrícula válida.\n");
        return;
    }
    file = fopen("arquivo_aluno", "r+b");
    if (file == NULL) {
        printf("Erro ao tentar abrir o arquivo! \n");
        return;
    }
    fseek(file, aluno * sizeof(Aluno), SEEK_SET); 
    fread(&al, sizeof(Aluno), 1, file);
    printf("Aluno encontrado!\n");
    printf("Alterando dados do aluno %s de matrícula %s.\n", al.nome, matricula);
    printf("Novo nome do aluno: ");
    fgets(al.nome, sizeof(al.nome), stdin);
    al.nome[strcspn(al.nome, "\n")] = '\0';
    if (nome(al.nome) != 1) {
        printf("O nome inserido é inválido!\n");
        fclose(file);
        return;
    }
    printf("\nEmail: ");
    fgets(al.email, sizeof(al.email), stdin);
    al.email[strcspn(al.email, "\n")] = '\0';
    verificar = email(al.email);
    if (verificar != 1) {
        printf("O E-mail não é válido! Tente novamente.\n");
        fclose(file);
        return;
    }
    printf("\nTelefone: ");
    fgets(al.telefone, sizeof(al.telefone), stdin);
    al.telefone[strcspn(al.telefone, "\n")] = '\0';
    if (telefone(al.telefone) != 1) {
        printf("O telefone inserido não é válido! \n");
        fclose(file);
        return;
    }
    fseek(file, aluno * sizeof(Aluno), SEEK_SET); 
    fwrite(&al, sizeof(Aluno), 1, file);
    fclose(file);
    printf("Dados alterados com sucesso!\n");
}

void exibir_dados(char matricula[]) {
    FILE *file;
    Aluno al;
    int ver = consulta_aluno(matricula);

    if (ver == -1) {
        printf("Aluno não encontrado. \n");
        return;
    }

    file = fopen("arquivo_aluno", "r+b");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo. \n");
        return;
    }
    fseek(file, ver * sizeof(Aluno), SEEK_SET);  
    if (fread(&al, sizeof(Aluno), 1, file) != 1) {
        printf("Erro ao ler os dados do aluno.\n");
        fclose(file);
        return;
    }
    printf("Nome do aluno: %s\n", al.nome);
    printf("E-mail: %s\n", al.email);
    printf("Telefone: %s\n", al.telefone);
    printf("Quantidade de disciplinas que está matriculado atualmente: %d\n", al.qnt_disciplinas);
    fclose(file);  
}

void remover_aluno(char* email_procurado) {
    FILE *file, *temporario;
    Aluno al;
    int encontrado = 0;

    file = fopen("arquivo_aluno", "rb");
    if (file == NULL) {
        printf("Erro ao tentar abrir o arquivo.\n");
        return;
    }
    temporario = fopen("arquivoTemporario_aluno", "wb");
    if (temporario == NULL) {
        printf("Erro ao criar o arquivo temporário.\n");
        fclose(file);
        return;
    }
    while (fread(&al, sizeof(Aluno), 1, file) == 1) {
        if (strcmp(email_procurado, al.email) == 0) {
            printf("O aluno de e-mail %s foi encontrado e será removido.\n", email_procurado);
            encontrado = 1;
        } else {
            fwrite(&al, sizeof(Aluno), 1, temporario);
        }
    }
    fclose(file);
    fclose(temporario);
    if (encontrado) {
        remove("arquivo_aluno");
        rename("arquivoTemporario_aluno", "arquivo_aluno");
        printf("Aluno removido com sucesso.\n");
    } else {
        remove("arquivoTemporario_aluno");
        printf("O aluno com e-mail %s não foi encontrado.\n", email_procurado);
    }
}

int consulta_disciplina(char* codigo_procurado) { 
    int i = 0;
    FILE *file; 
    Disciplina dis;
    file = fopen("arquivo_disciplina", "rb"); 
    if (file == NULL) {
        printf("Erro ao tentar abrir o arquivo 'arquivo_disciplina'.\n");
        return -2; 
    }
    fseek(file, 0, SEEK_SET);
    while (fread(&dis, sizeof(Disciplina), 1, file) == 1) {
        if (strcmp(dis.codigo_disciplina, codigo_procurado) == 0) {
            fclose(file); 
            return i;  
        }
        i++;  
    }
    fclose(file);
    return -1;  
}

void cadastrar_disciplina() {
    Disciplina dis;
    char input[256];
    int encontrado = 0;
    char codigo_procurado[8];
    FILE *file_disciplina;

    file_disciplina = fopen("arquivo_disciplina", "a+b");
    if(file_disciplina == NULL){
        printf("Erro ao abrir/criar arquivo. \n");
        return;
    }
    printf("Insira o código da disciplina: ");
    fgets(codigo_procurado, sizeof(codigo_procurado), stdin);
    codigo_procurado[strcspn(codigo_procurado, "\n")] = '\0';
    fseek(file_disciplina, 0, SEEK_SET); 
    while (fread(&dis, sizeof(Disciplina), 1, file_disciplina) == 1) {
        if (strcmp(dis.codigo_disciplina, codigo_procurado) == 0) {
            printf("Erro: A disciplina com este código já existe!\n");
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        strcpy(dis.codigo_disciplina, codigo_procurado);
        printf("Insira o nome da disciplina: ");
        fgets(dis.nome_disciplina, sizeof(dis.nome_disciplina), stdin);

        dis.nome_disciplina[strcspn(dis.nome_disciplina, "\n")] = '\0';
        printf("Insira o horário da disciplina: Exemplo: (08:00AM - 12:00PM)\n");
        fgets(dis.horario_disciplina, sizeof(dis.horario_disciplina), stdin);
        dis.horario_disciplina[strcspn(dis.horario_disciplina, "\n")] = '\0';

        printf("Sala de aula onde ocorrerá a disciplina: ");
        fgets(input, sizeof(input), stdin);
        if (sscanf(input, "%d", &dis.sala_aula) != 1) {
            printf("Número de sala inválido!\n");
            fclose(file_disciplina);
            return;
        }
        printf("Quantidade de vagas: ");
        fgets(input, sizeof(input), stdin);
        if (sscanf(input, "%d", &dis.qnt_vagasTotal) != 1) {
            printf("Quantidade de vagas inválida!\n");
            fclose(file_disciplina);
            return;
        }
        printf("Quantidade de vagas ocupadas: ");
        fgets(input, sizeof(input), stdin);
        if (sscanf(input, "%d", &dis.qnt_vagasOcupadas) != 1) {
            printf("Quantidade de vagas ocupadas inválida!\n");
            fclose(file_disciplina);
            return;
        }
        if (dis.qnt_vagasOcupadas > dis.qnt_vagasTotal) {
            printf("Quantidade de vagas ocupadas não pode exceder a total.\n");
            fclose(file_disciplina);
            return;
        }
        printf("Qual o status da disciplina: (1 - ativo, 0 - deletado) ");
        fgets(input, sizeof(input), stdin);
        if (sscanf(input, "%d", &dis.status) != 1) {
            printf("Status inválido!\n");
            fclose(file_disciplina);
            return;
        }
        fseek(file_disciplina, 0, SEEK_END); 
        fwrite(&dis, sizeof(Disciplina), 1, file_disciplina);
        printf("Disciplina cadastrada com sucesso.\n");
    }

    fclose(file_disciplina);
}

void alterar_disciplina() {
    char codigo[9];  
    FILE *file;
    Disciplina dis;
    int pos;

    printf("Digite o código da disciplina que deseja alterar: ");
    fgets(codigo, sizeof(codigo), stdin);
    codigo[strcspn(codigo, "\n")] = '\0';
    pos = consulta_disciplina(codigo);
    if (pos == -1) {
        printf("Disciplina não encontrada.\n");
        return;
    }
    file = fopen("arquivo_disciplina", "r+b");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }
    fseek(file, pos * sizeof(Disciplina), SEEK_SET);
    fread(&dis, sizeof(Disciplina), 1, file);
    printf("Nome atual: %s\n", dis.nome_disciplina);
    printf("Novo nome: ");
    fgets(dis.nome_disciplina, sizeof(dis.nome_disciplina), stdin);
    dis.nome_disciplina[strcspn(dis.nome_disciplina, "\n")] = '\0';
    if (strlen(dis.nome_disciplina) == 0) {
        printf("Nome da disciplina não pode ser vazio.\n");
        fclose(file);
        return;
    }
    fseek(file, pos * sizeof(Disciplina), SEEK_SET);
    if (fwrite(&dis, sizeof(Disciplina), 1, file) != 1) {
        printf("Erro ao salvar as alterações.\n");
        fclose(file);
        return;
    }
    fclose(file);
    printf("Disciplina alterada com sucesso!\n");
}

void exibir_disciplina() {
    FILE *file;
    Disciplina dis;
    char codigo_procurado[8];

    printf("Digite o código da disciplina que deseja EXIBIR: ");
    fgets(codigo_procurado, sizeof(codigo_procurado), stdin);
    codigo_procurado[strcspn(codigo_procurado, "\n")] = '\0';

    int pos = consulta_disciplina(codigo_procurado);
    if (pos == -1) {
        printf("Disciplina não encontrada.\n");
        return;
    }

    file = fopen("arquivo_disciplina", "rb");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    fseek(file, pos * sizeof(Disciplina), SEEK_SET);
    if (fread(&dis, sizeof(Disciplina), 1, file) != 1) {
        printf("Erro ao ler a disciplina no arquivo.\n");
        fclose(file);
        return;
    }
    printf("\nCódigo: %s", dis.codigo_disciplina);
    printf("\nNome: %s", dis.nome_disciplina);
    printf("\nHorário: %s", dis.horario_disciplina);
    printf("\nSala: %d", dis.sala_aula);
    printf("\nVagas Totais: %d", dis.qnt_vagasTotal);
    printf("\nVagas Ocupadas: %d", dis.qnt_vagasOcupadas);
    printf("\nStatus: %s", (dis.status == 1) ? "Ativo" : "Inativo");
    fclose(file);
}

void remover_disciplina() {
    FILE *file, *temporario;
    Disciplina dis;
    char codigo[8];
    int encontrado = 0;

    printf("Digite o código da disciplina que deseja remover: ");
    fgets(codigo, sizeof(codigo), stdin);
    codigo[strcspn(codigo, "\n")] = '\0';
    int pos = consulta_disciplina(codigo);
    if (pos == -1) {
        printf("Disciplina não encontrada.\n");
        return;
    }
    file = fopen("arquivo_disciplina", "rb");
    if (file == NULL) {
        printf("Erro ao tentar abrir o arquivo de disciplinas.\n");
        return;
    }
    temporario = fopen("arquivoTemporario_disciplina", "wb");
    if (temporario == NULL) {
        printf("Erro ao criar o arquivo temporário.\n");
        fclose(file);
        return;
    }
    while (fread(&dis, sizeof(Disciplina), 1, file) == 1) {
        if (strcmp(dis.codigo_disciplina, codigo) != 0) {
            fwrite(&dis, sizeof(Disciplina), 1, temporario);
        } else {
            encontrado = 1;
            printf("A disciplina de código %s foi encontrada e será removida.\n", codigo);
        }
    }

    fclose(file);
    fclose(temporario);
    if (encontrado) {
        remove("arquivo_disciplina");
        rename("arquivoTemporario_disciplina", "arquivo_disciplina");
        printf("Disciplina removida com sucesso.\n");
    } else {
        remove("arquivoTemporario_disciplina");
        printf("Disciplina não encontrada.\n");
    }
}

void incluir_matricula() {
    FILE *matricula;
    Matricula mat;

    matricula = fopen("arquivo_matricula", "a+b");
    if (matricula == NULL) {
        printf("Erro ao tentar abrir/criar o arquivo.\n");
        return;
    }
    printf("\nInsira o código da disciplina que você deseja matricular: ");
    fgets(mat.codigo_disciplina, 9, stdin);
    mat.codigo_disciplina[strcspn(mat.codigo_disciplina, "\n")] = '\0';
    if (consulta_disciplina(mat.codigo_disciplina) == -1) {
        printf("Esse código de disciplina não existe. Tente novamente.\n");
        fclose(matricula);
        return;
    }
    printf("\nMatrícula do aluno: ");
    fgets(mat.matricula, 12, stdin);
    mat.matricula[strcspn(mat.matricula, "\n")] = '\0';
    if (consulta_aluno(mat.matricula) == -1) {
        printf("Esse aluno não existe!\n");
        fclose(matricula);
        return;
    }
    printf("\nData da matrícula (formato: DD/MM/AAAA): ");
    fgets(mat.data_matricula, 9, stdin);
    mat.data_matricula[strcspn(mat.data_matricula, "\n")] = '\0';
    printf("\nQual o status do registro? (1 - ativo / 0 - deletado): ");
    scanf(" %d", &mat.status);

    fwrite(&mat, sizeof(Matricula), 1, matricula);
    fclose(matricula);
    printf("Matrícula realizada com sucesso!\n");
}

int consulta_matricula(char matricula_procurada[]) {
    int i = 0;
    FILE *file;
    Matricula mat;

    file = fopen("arquivo_matricula", "rb");
    if (file == NULL) {
        printf("Erro ao tentar abrir o arquivo.\n");
        return -2;
    }
    while (fread(&mat, sizeof(Matricula), 1, file)) {
        if (strcmp(mat.matricula, matricula_procurada) == 0) {
            fclose(file);
            return i;
        }
        i++; 
    }
    fclose(file);
    return -1; 
}


void excluir_matricula(char *matricular) {
    FILE *matricula;
    Matricula mat;
    char matricula_aluno[12]; 
    int pos;

    matricula = fopen("arquivo_matricula", "r+b");
    if (matricula == NULL) {
        printf("Erro ao tentar abrir o arquivo.\n");
        return;
    }
    printf("Digite a matrícula do aluno que deseja excluir: ");
    fgets(matricula_aluno, 12, stdin);
    matricula_aluno[strcspn(matricula_aluno, "\n")] = '\0';
    pos = consulta_matricula(matricula_aluno); 
    if (pos == -1) {
        printf("Aluno não encontrado ou matrícula não encontrada.\n");
        fclose(matricula);
        return;
    }
    fseek(matricula, pos * sizeof(Matricula), SEEK_SET);
    fread(&mat, sizeof(Matricula), 1, matricula);
    mat.status = 0;
    fseek(matricula, pos * sizeof(Matricula), SEEK_SET);
    fwrite(&mat, sizeof(Matricula), 1, matricula);

    fclose(matricula);
    printf("Matrícula excluída com sucesso!\n");
}

int main() {
    int op, op_disciplina, op_aluno, op_matricula;
    Aluno al;
    Matricula mat;

    do {
        printf("\nSelecione o módulo a ser trabalhado:\n");
        menu();
        scanf("%d", &op);
        getchar(); 

        switch (op) {
            case 1:
                printf("\nVocê escolheu a opção 'Disciplina'!\n");
                printf("Selecione uma opção:\n");
                menu_disciplina();
                scanf("%d", &op_disciplina);
                getchar();
                switch (op_disciplina){
                    case 1:
                        printf("Você escolheu a opção 'Cadastrar disciplina'. \n");
                        cadastrar_disciplina();
                        break;
                    case 2:
                        printf("Você escolheu a opção 'Excluir disciplina'. \n");
                        remover_disciplina();
                        break;
                    case 3:
                        printf("Você escolheu a opção 'Alterar dados da disciplina'. \n");
                        alterar_disciplina();
                        break;
                    case 4:
                        printf("Você escolheu a opção 'Exibir dados de uma disciplina'. \n");
                        exibir_disciplina();  
                        break;
                    case 5:
                        printf("Você escolheu a opção 'Retornar ao menu principal'. \n");
                        break;
                    default:
                        printf("Opção inválida. Tente novamente.\n");
                }
                break;
            case 2:
                printf("\nVocê escolheu a opção 'Aluno'!\n");
                printf("Selecione uma opção:\n");
                menu_aluno();
                scanf("%d", &op_aluno);
                getchar(); 
                switch (op_aluno) {
                    case 1:
                        printf("Você escolheu a opção 'Cadastrar novo aluno'.\n");
                        cadastrar_aluno();  
                        break;
                    case 2:
                        printf("Você escolheu a opção 'Alterar dados de um aluno'.\n");
                        printf("Digite a matricula do aluno que deseja modificar os dados:");
                        fgets(al.matricula, 12, stdin);
                        alterar_dados(al.matricula);  
                        break;
                    case 3:
                        printf("Você escolheu a opção 'Exibir dados de um aluno'.\n");
                        printf("Digite a matricula do aluno que deseja exibir os dados:");
                        fgets(al.matricula, 12, stdin);
                        al.matricula[strcspn(al.matricula, "\n")] = '\0';
                        exibir_dados(al.matricula);  
                        break;
                    case 4:
                        printf("Você escolheu a opção 'Remover aluno'.\n");
                        printf("Insira o e-email do aluno que deseja remover: ");
                        fgets(al.email, 100, stdin);
                        al.email[strcspn(al.email, "\n")] = '\0';
                        remover_aluno(al.email); 
                        break;
                    case 5:
                        printf("Retornando ao menu principal.\n");
                        break;
                    default:
                        printf("Opção inválida. Tente novamente.\n");
                }
                break;
            case 3:
                printf("\nVocê escolheu a opção 'Matrícula'!\n");
                printf("Selecione uma opção:\n");
                menu_matricula();
                scanf("%d", &op_matricula);
                getchar();
                switch (op_matricula){
                    case 1: 
                        printf("Você escolheu a opção 'Inclusão de matrícula'. \n");
                        incluir_matricula();  
                        break;
                    case 2:
                         printf("Você escolheu a opção 'Exclusão de matrícula'. \n");
                         printf("Digite a matricula que deseja excluir: ");
                         fgets(mat.matricula, 12, stdin);
                         excluir_matricula(mat.matricula); 
                         break;
                    case 3: 
                        printf("Você escolheu a opção 'Retornar ao menu principal'. \n");
                        break;
                    default:
                        printf("Opção inválida. Tente novamente. \n");
                }
                break;
            case 4:
                printf("Programa finalizado.\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    } while (op != 4);
    //remoção
    int status;
    status = remove("arquivo_aluno");
    if(status != 0){
        printf("'arquivo_aluno' não foi removido devidamente. \n");
    }
    else{
        printf("'arquivo_aluno' deletado definitivamente. \n");
    }

    int status_disciplina;
    status_disciplina = remove("arquivo_disciplina");
    if(status_disciplina != 0){
        printf("'arquivo_disciplina' não foi removido devidamente. \n");
    }
    else{
        printf("'arquivo_disciplina' deletado definitivamente. \n");
    }

    int status_matricula;
    status_matricula = remove("arquivo_matricula");
    if(status_disciplina != 0){
        printf("'arquivo_matricula' não foi removido devidamente. \n");
    }
    else{
        printf("'arquivo_matricula' deletado definitivamente. \n");
    }
    return 0;
}
