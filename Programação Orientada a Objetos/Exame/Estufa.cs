using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.Serialization.Formatters.Binary;

namespace Grupo1
{
    /// <summary>
    /// Definição de estufa
    /// </summary>
    [Serializable]
    public class Estufa
    {
        #region Atributos
        public string Codigo { get; set; }
        public string Tipo { get; set; }
        public string Morada { get; set; }
        public double CapacidadeProducao { get; set; }
        public List<Intervencao> Intervencoes { get; set; }

        #endregion

        #region Propriedades

        #region Construtores
        /// <summary>
        /// Construtor por defeito
        /// </summary>
        public Estufa()
        {
        }

        /// <summary>
        /// Construtor de estufa
        /// </summary>
        /// <param name="codigo"></param>
        /// <param name="tipo"></param>
        /// <param name="morada"></param>
        /// <param name="capacidadeProducao"></param>
        public Estufa(string codigo, string tipo, string morada, double capacidadeProducao)
        {
            Codigo = codigo;
            Tipo = tipo;
            Morada = morada;
            CapacidadeProducao = capacidadeProducao;
            Intervencoes = new List<Intervencao>();
        }
        #endregion

        #endregion
    }
}
