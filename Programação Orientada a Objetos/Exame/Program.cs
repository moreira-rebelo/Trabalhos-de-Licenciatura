/*
 * Exame POO - Época Especial
 * 2023/07/18
 * Francisco Moreira Rebêlo, 16443
 * */

using System;
using System.Runtime.Serialization.Formatters.Binary;

namespace Grupo1
{
    internal class Program
    {
        static void Main(string[] args)
        {
            // Criar uma nova empresa
            Empresa empresa = new Empresa();

            // Registar novas estufas
            Estufa estufa1 = new Estufa { Codigo = "E001", Tipo = "flores", Morada = "Rua A", CapacidadeProducao = 1000 };
            Estufa estufa2 = new Estufa { Codigo = "E002", Tipo = "hortícolas", Morada = "Rua B", CapacidadeProducao = 1500 };


           empresa.ResgistarEstufa(estufa1); //Se já existe, retorna false. Se adicionou, retorna true
           empresa.ResgistarEstufa(estufa2);

            //Verificar estufas de determinado tipo com capacidade acima de um valor
            double capacidadeMinima = 1200;
            string tipoEstufa = "flores";

            var estufasFiltradas = empresa.VerificarEstufasPorTipoECapacidade(tipoEstufa, capacidadeMinima);
            string estufasFiltradasMensagem = $"Estufas do tipo '{tipoEstufa}' com capacidade acima de {capacidadeMinima} kg/ano:\n";
            foreach (var estufa in estufasFiltradas)
            {
                estufasFiltradasMensagem += $"Código: {estufa.Codigo}, Tipo: {estufa.Tipo}, Capacidade: {estufa.CapacidadeProducao}\n";
            }

            // Identificar estufas operacionais em determinada região, ordenadas pela capacidade de produção
            string regiao = "Rua";

            var estufasOperacionais = empresa.IdentificarEstufasPorRegiaoOrdenadasPorCapacidade(regiao);
            string estufasOperacionaisMensagem = $"Estufas operacionais na região '{regiao}', ordenadas pela capacidade de produção:\n";
            foreach (var estufa in estufasOperacionais)
            {
                estufasOperacionaisMensagem += $"Código: {estufa.Codigo}, Capacidade: {estufa.CapacidadeProducao}\n";
            }

            // Verificar se determinada estufa existe e retornar suas informações
            string codigoEstufa = "E001";

            var estufaEncontrada = empresa.VerificarEstufa(codigoEstufa);
            string estufaEncontradaMensagem = estufaEncontrada != null ? $"Estufa encontrada - Código: {estufaEncontrada.Codigo}, Tipo: {estufaEncontrada.Tipo}" : "Estufa não encontrada.";

            //Guardar em ficheiro binário
            string fileName = @"C:\Users\morei\Desktop\ExameEpocaEspecial_POO_16443\Grupo1\Grupo1\estufas.bin";//Caminho do ficheiro binário
            empresa.SaveAll(fileName);
            
            //Registar intervenção
            DateTime dataIntervencao1 = DateTime.Now; //Data da intervenção
            string descricaoIntervencao1 = "Substituição do veniador de arejamento"; // Descrição da intervenção
            Intervencao intervencao1 = new Intervencao(dataIntervencao1, descricaoIntervencao1);
            estufa1.Intervencoes.Add(intervencao1);

            DateTime dataIntervencao2 = new DateTime(2023, 7, 18, 21, 00, 0); // Ano, mês, dia, hora, minutos, segundos
            string descricaoIntervencao2 = "Purga do sistema de rega";
            Intervencao intervencao2 = new Intervencao(dataIntervencao2, descricaoIntervencao2);
            estufa2.Intervencoes.Add(intervencao2);
        }
    }
}
