using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.Serialization.Formatters.Binary;

namespace Grupo1
{
    /// <summary>
    /// Alínea d
    /// Classe interveção que irá registar as intervenções às estufas
    /// </summary>
    [Serializable]
    public class Intervencao
    {

        #region Atributos
        public DateTime Data { get; set; }
        public string Descricao { get; set; }

        #endregion

        #region Propriedades

        #region Construtores

        /// <summary>
        /// Construtor de intervencao
        /// </summary>
        /// <param name="data"></param>
        /// <param name="descricao"></param>
        public Intervencao(DateTime data, string descricao)
        {
            Data = data;
            Descricao = descricao;
        }

        #endregion

        #endregion
    }

}
