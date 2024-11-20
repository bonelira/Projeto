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
    printf("Disciplina - 1\n");
    printf("Aluno - 2\n");
    printf("Matrículas - 3\n");
    printf("Finalizar programa - 4\n");
}

void menu_disciplina() {
    printf("Cadastrar nova disciplina - 1\n");
    printf("Excluir disciplina existente - 2\n");
    printf("Alterar dados da disciplina - 3\n");
    printf("Exibir dados de uma disciplina - 4\n");
    printf("Retornar ao menu principal - 5\n");
}

void menu_aluno() {
    printf("Cadastrar novo aluno - 1\n");
    printf("Alterar dados de um aluno - 2\n");
    printf("Exibir dados de um aluno - 3\n");
    printf("Remover aluno - 4\n");
    printf("Retornar ao menu principal - 5\n");
}

void menu_matricula() {
    printf("Inclusão de matrícula - 1\n");
    printf("Exclusão de matrícula - 2\n");
    printf("Retornar ao menu principal - 3\n");
}

int consulta_aluno(const char* matricula_procurada) { //esse tipo de parametro deixa claro que é apenas para leitura e que a matricula recebida não pode ser alterada ao longo da função
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
    int aluno = consulta_aluno(matricula), i, verificar;
    Aluno al;
    FILE *file;
    if (aluno == -1) {
        printf("A matrícula inserida não existe! Digite uma matrícula válida.\n");
        return;
    }
    file = fopen("arquivo_aluno", "r+b");
    if (file == NULL){
        printf("Erro ao tentar abrir o arquivo! \n");
        return;
    }
    fseek(file, aluno * sizeof(Aluno), SEEK_SET);
    fread(&al, sizeof(Aluno), 1, file);

    printf("Aluno encontrado!\n");
    printf("Alterando dados do aluno %s de matrícula %s.\n", al.nome, matricula);
    printf("Novo nome do aluno: ");
    fgets(al.nome, 250, stdin);
    al.nome[strcspn(al.nome, "\n")] = '\0';
    for (i = 0; al.nome[i] != '\0'; i++) {
        if (!isalpha(al.nome[i]) && !isspace(al.nome[i])) {
            printf("O nome que foi inserido é inválido!\n");
            fclose(file);
            return;
        }
    }
    printf("\nEmail: ");
    fgets(al.email, 100, stdin);
    al.email[strcspn(al.email, "\n")] = '\0';
    verificar = email(al.email);
    if (verificar != 1){
        printf("O E-mail não é válido! Tente novamente. \n");
        return;
    }
    printf("\nTelefone: ");
    fgets(al.telefone, 15, stdin);
    al.telefone[strcspn(al.telefone, "\n")] = '\0';
    fseek(file, aluno * sizeof(Aluno), SEEK_SET);
    fwrite(&al, sizeof(Aluno), 1, file);
    fclose(file);
    printf("Dados alterados com sucesso!\n");
}

void exibir_dados(char matricula[]){
    FILE *file;
    Aluno al;
    int ver = consulta_aluno(matricula);
    if(ver == -1){
        printf("Aluno não encontrado. \n");
        return;
    }
     file = fopen("arquivo_aluno", "rb");
    if (file == NULL){
        printf("Erro ao abrir o arquivo. \n");
        return;
    }
    fseek(file, ver * sizeof(Aluno), SEEK_SET);
    fread(&al, sizeof(Aluno), 1, file);
    if (fread(&al, sizeof(Aluno), 1, file) != 1) {
        printf("Erro ao ler os dados do aluno.\n");
        fclose(file);
        return;
    }
    printf("Nome do aluno: %s\n", al.nome);
    printf("E-mail: %s\n", al.email);
    printf("Telefone: %s\n", al.telefone);
    printf("Quantidade de disciplinas que está matriculado: %d\n", al.qnt_disciplinas);
    fclose(file);
}

void remover_aluno(const char* email_procurado) {
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


int consulta_disciplina(const char* codigo_procurado) { 
    int i = 0;
    FILE *file; 
    Disciplina dis;

    file = fopen("disciplina_arquivos", "rb");
    if (file == NULL) {
        printf("Erro ao tentar abrir o arquivo.\n");
        return -2;
    }

    while (fread(&dis, sizeof(Disciplina), 1, file)) {
        if (strcmp(dis.codigo_disciplina, codigo_procurado) == 0) {
            fclose(file);
            return i;
        }
        i++; 
    }

    fclose(file);
    return -1;
}

void cadastrar_disciplina(FILE *cadastrar_disciplina){
    Disciplina dis;

    printf("Insira o código da disciplina: ");
    fgets(dis.codigo_disciplina, 9, stdin);
    dis.codigo_disciplina[strcspn(dis.codigo_disciplina, "\n")] = '\0';
    int consultar = consulta_disciplina(dis.codigo_disciplina);
    if(consultar != -1){
        printf("Esse código já existe! \n");
        return;
    }
    printf("Insira o nome da disciplina: ");
    fgets(dis.nome_disciplina, 250, stdin);
    dis.nome_disciplina[strcspn(dis.nome_disciplina, "\n")] = '\0'; 
    if (nome(dis.nome_disciplina)!= 1){
        printf("O nome inserido não é válido! \n");
        return;
    }
    printf("Insira o horário da disciplina:  Exemplo: (08:00AM - 12:00PM)");
    fgets(dis.horario_disciplina, 250, stdin); //verificar se é valido
    dis.horario_disciplina[strcspn(dis.horario_disciplina, "\n")] = '\0';
    printf("Sala de aula onde ocorrerá a disciplina: ");
    scanf(" %d", &dis.sala_aula);
    printf("Quantidade de vagas: ");
    scanf(" %d", &dis.qnt_vagasTotal);
    printf("Quantidade de vagas ocupadas: ");
    scanf(" %d", &dis.qnt_vagasOcupadas);
    if (dis.qnt_vagasOcupadas > dis.qnt_vagasTotal) {
        printf("Quantidade de vagas ocupadas não pode exceder a total.\n");
        return;
    }
    printf("Qual o status da disciplina: (1 - ativo, 0 - deletado) ");
    scanf(" %d", &dis.status);
    
    fwrite(&dis, sizeof(Disciplina), 1, cadastrar_disciplina);
    printf("Disciplina cadastrada com sucesso!\n");

}

void alterar_disciplina() {
    char codigo[8];
    Disciplina dis;
    int pos;

    printf("Digite o código da disciplina que deseja alterar: ");
    fgets(codigo, 8, stdin);
    codigo[strcspn(codigo, "\n")] = '\0';

    pos = consulta_disciplina(codigo);
    if (pos == -1) {
        printf("Disciplina não encontrada.\n");
        return;
    }

    FILE *file = fopen("arquivo_disciplina", "r+b");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    fseek(file, pos * sizeof(Disciplina), SEEK_SET);
    fread(&dis, sizeof(Disciplina), 1, file);

    printf("Nome atual: %s\nNovo nome: ", dis.nome_disciplina);
    fgets(dis.nome_disciplina, 250, stdin);
    dis.nome_disciplina[strcspn(dis.nome_disciplina, "\n")] = '\0';

    fseek(file, pos * sizeof(Disciplina), SEEK_SET);
    fwrite(&dis, sizeof(Disciplina), 1, file);
    fclose(file);
    printf("Disciplina alterada com sucesso!\n");
}

void exibir_disciplina(){

}

void remover_disciplina(){

}

void incluir_matricula(){
    FILE *matricula;
    Matricula mat;

    matricula = fopen("arquivo_matricula", "a+b");
    if(matricula == NULL){
        printf("Erro ao tentar abrir/criar o arquivo. \n");
        return;
    }
    printf("\nInsira o código da disciplina que você deseja se matricular: ");
    fgets(mat.codigo_disciplina, 9, stdin);
    mat.codigo_disciplina[strcspn(mat.codigo_disciplina, "\n")] = '\0';
    if (consulta_disciplina(mat.codigo_disciplina) == -1){
        printf("Esse código não existe. Tente novamente. \n");
        fclose(matricula);
        return;
    }
    printf("\nMatricula do aluno: ");
    fgets(mat.matricula, 12, stdin);
    mat.matricula[strcspn(mat.matricula, "\n")] = '\0';
    if(consulta_aluno(mat.matricula) == -1){
        printf("Esse aluno não existe! \n");
        fclose(matricula);
        return;
    }
    printf("\nData de matricula: ");
    fgets(mat.data_matricula, 9, stdin);
    mat.data_matricula[strcspn(mat.data_matricula, "\n")] = '\0';
    printf("\nQual o status do registro? ex: (1 - ativo/0 - deletado)");
    scanf(" %d", &mat.status);

    fclose(matricula);
    printf("Cadastro realizado com sucesso! \n");
}

void excluir_matricula(){

}

int main() {
    FILE *file_cadastroaluno;
    FILE  *disciplina;
    FILE *matricula;
    int op, op_disciplina, op_aluno, op_matricula;
    Aluno al;

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
                        disciplina = fopen("arquivo_disciplina", "a+b");
                        if(disciplina == NULL){
                            printf("Erro ao abrir/criar o arquivo! \n");
                            return 1;
                        }
                        else{
                            cadastrar_disciplina(disciplina);
                            fclose(disciplina);
                        }
                        break;
                    case 2:
                        printf("Você escolheu a opção 'Excluir disciplina'. \n");
                        break;
                    case 3:
                        printf("Você escolheu a opção 'Alterar dados da disciplina'. \n");
                        break;
                    case 4:
                        printf("Você escolheu a opção 'Exibir dados de uma disciplina'. \n");
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
                        exibir_dados(al.matricula);
                        break;
                    case 4:
                        printf("Você escolheu a opção 'Remover aluno'.\n");
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
                        break;
                    case 2:
                         printf("Você escolheu a opção 'Exclusão de matrícula'. \n");
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
    return 0;
}
