/**
 * PROJETO PROGRAMAÇÃO ORIENTADA A OBJETOS - PRIMEIRA FASE
 * ALUNOS: Allan Aleluia (21990)||Francisco Rebêlo (a16443)||Rui Magalhães (a22300)
 * DOCENTE: Luís Ferreira
 * 
 * Classe PESSOA - Para criação de uma pessoa
 **/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BusIPCA
{
    /// <summary>
    /// Classe pessoa e seus atributos
    /// </summary>
    internal class Pessoa
    {
        private int numMec;
        private string nome;
        private int telemovel;
        private string email;


        #region METODOS

        #region CONSTRUTORES

        /// <summary>
        /// Método genérico para criar uma pessoa
        /// </summary>
        public Pessoa()
        {
            this.NumMec = 0;
            this.nome = "";
            this.Telemovel = 0;
            this.email = "";
        }

        /// <summary>
        /// Método para criar uma pessoa
        /// </summary>
        /// <param name="idPessoa"></param>
        /// <param name="nome"></param>
        /// <param name="telemovel"></param>
        /// <param name="email"></param>
        public Pessoa(int idPessoa, string nome, int telemovel, string email)
        {
            this.NumMec = idPessoa;
            this.nome = nome;
            this.Telemovel = telemovel;
            this.email = email;
        }


        #endregion

        #region PROPRIEDADES  
        /*
         * propriedades para alterarem os dados do objeto criado sem acessar a raiz(classe), tornando a criação mais intuitiva.
         */
        public int NumMec
        {
            get { return numMec; }
            set { this.numMec = value; }
        }

        public string Nome
        {
            get { return nome; }
            set { this.nome = value; }
        }

        public int Telemovel
        {
            get { return telemovel; }
            set
            {
                //Exception que verifica se o numero de telemóvel tem 9 dígitos
                try
                {
                    
                    if (value.ToString().Length != 9)
                    {
                        throw new Exception("O número de telemóvel tem de ter 9 dígitos!");
                    }
                    this.telemovel = value;
                }
                catch (Exception ex)
                {
                    //Se não
                    Console.WriteLine(ex.Message);
                }
            }
        }

        public string Email
        {
            get { return email; }
            set { this.email = value; }
        }

        #region OPERADORES

        /// <summary>
        /// Reescrveendo o método equals para comparar se duas pessoas são iguais
        /// </summary>
        /// <param name="p1"></param>
        /// <param name="p2"></param>
        /// <returns></returns>
        public static bool operator ==(Pessoa p1, Pessoa p2)
        {
            return (p1.Equals(p2));
        }

        /// <summary>
        /// reescrevendo o método equals para comparar se duas pessoas são diferentes
        /// </summary>
        /// <param name="p1"></param>
        /// <param name="p2"></param>
        /// <returns></returns>
        public static bool operator !=(Pessoa p1, Pessoa p2)
        {
            return (!(p1 == p2));
        }

        #region OVERRIDE

        /// <summary>
        /// Atribui um número mecanografico a Pessoa
        /// </summary>
        /// <param name="obj"></param>
        /// <returns></returns>
        public override bool Equals(object obj)
        {
            Pessoa p = obj as Pessoa;
            return p.NumMec == this.NumMec;
        }

        /// <summary>
        /// Feito o hashcode apra garantir que seja verdadeiro o override do método equals
        /// </summary>
        /// <returns></returns>
        public override int GetHashCode()
        {
            return 0;
        }

        #endregion

        #endregion

        #endregion

        #endregion
    }
}
