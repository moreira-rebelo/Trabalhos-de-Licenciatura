using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization.Formatters.Binary;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.Serialization.Formatters.Binary;

namespace Grupo1
{
    /// <summary>
    /// Empresa constituída por estufas
    /// </summary>
    [Serializable]
    public class Empresa
    {
        #region Atributos
        public List<Estufa> Estufas { get; set; }

        #endregion

        #region Propriedades

        #region Construtores
        /// <summary>
        /// Construtor de empresa
        /// </summary>
        /// <param name="estufas"></param>
        public Empresa(List<Estufa> estufas)
        {
            Estufas = estufas;
        }

        #endregion

        #region Outros Metodos

        /// <summary>
        /// Alínea b, pergunta I
        /// Adicionar novas estufas
        /// </summary>
        /// <param name="estufa"></param>
        /// <returns></returns>
        public bool ResgistarEstufa(Estufa estufa)
        {
            if (Estufas.Exists(e => e.Codigo == estufa.Codigo))
            {
                return false;
            }

            Estufas.Add(estufa);
            return true;
        }

        /// <summary>
        /// Alínea b, pergunta II
        /// Verificar estufas de determinado tipo com capacidade acima de um valor
        /// </summary>
        /// <param name="tipo"></param>
        /// <param name="capacidadeMinima"></param>
        /// <returns></returns>
        public List<Estufa> VerificarEstufasPorTipoECapacidade(string tipo, double capacidadeMinima)
        {
            List<Estufa> estufasFiltradas = new List<Estufa>();

            foreach (Estufa estufa in Estufas)
            {
                if (estufa.Tipo == tipo && estufa.CapacidadeProducao > capacidadeMinima)
                {
                    estufasFiltradas.Add(estufa);
                }
            }

            return estufasFiltradas;
        }

        /// <summary>
        /// Alínea b, pergunta III
        /// Identificar estufas operacionais em determinada região, ordenadas pela capacidade de produção
        /// </summary>
        /// <param name="regiao"></param>
        /// <returns></returns>
        public List<Estufa> IdentificarEstufasPorRegiaoOrdenadasPorCapacidade(string regiao)
        {
            List<Estufa> estufasOperacionais = new List<Estufa>();

            foreach (Estufa estufa in Estufas)
            {
                if (estufa.Morada.Contains(regiao))
                {
                    estufasOperacionais.Add(estufa);
                }
            }

            estufasOperacionais.Sort((e1, e2) => e2.CapacidadeProducao.CompareTo(e1.CapacidadeProducao));
            return estufasOperacionais;
        }

        /// <summary>
        /// Alínea b, pergunta IV
        /// Verificar se determinada estufa existe e retornar suas informações
        /// </summary>
        /// <param name="codigoEstufa"></param>
        /// <returns></returns>
        public Estufa VerificarEstufa(string codigoEstufa)
        {
            return Estufas.Find(e => e.Codigo == codigoEstufa);
        }

        /// <summary>
        /// Alínea b, pergunta V
        /// Guardar informação para ficheiro binário
        /// </summary>
        /// <param name="fileName"></param>
        /// <returns></returns>
        public bool SaveAll(string fileName)
        {
            try
            {
                Stream stream = File.Open(fileName, FileMode.OpenOrCreate);
                BinaryFormatter bin = new BinaryFormatter();
                bin.Serialize(stream, Estufas);
                stream.Close();
                return true;
            }
            catch (IOException e)
            {
                throw e;
            }
        }

        /// <summary>
        /// Criar uma nova lista de estufas em empresa
        /// </summary>
        public Empresa()
        {
            Estufas = new List<Estufa>();
        }

        #endregion

        #endregion

    }
}
