// ch 2022
// netstandard 2.0
// v1.0.0
// vrntzt
// c# api
// api for simplistic genotype/phenotype & corresponding evolution

using System;
using System.Runtime.InteropServices;
using System.Text;

namespace vrntzt.neat.csharp_api
{
    /// <summary>
    /// delegate for defining of output (out, debug, warning, error)
    /// parameter len can be ignored
    /// </summary>
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void OutputCallback(StringBuilder t_string, uint len);

    /// <summary>
    /// manages output - works via specified callback functions of type
    /// OutputCallback
    /// there are 4 different output streams: out, debug, warning and error
    /// </summary>
    public static class IO
    {
        /// <summary>
        /// set callback functions for all streams
        /// </summary>
        [DllImport("vrntzt_neat_c_dll_1_0.dll", EntryPoint = "set_all_streams")]
        public static extern void SetAllStreams(OutputCallback t_callback_f);

        /// <summary>
        /// set callback functions for output stream
        /// </summary>
        [DllImport("vrntzt_neat_c_dll_1_0.dll", EntryPoint = "set_out_stream")]
        public static extern void SetOutStream(OutputCallback t_callback_f);

        /// <summary>
        /// set callback functions for debug stream
        /// </summary>
        [DllImport("vrntzt_neat_c_dll_1_0.dll", EntryPoint = "set_debug_stream")]
        public static extern void SetDebugStream(OutputCallback t_callback_f);

        /// <summary>
        /// set callback functions for warnings
        /// </summary>
        [DllImport("vrntzt_neat_c_dll_1_0.dll", EntryPoint = "get_fset_warning_streamitness")]
        public static extern void SetWarningStream(OutputCallback t_callback_f);

        /// <summary>
        /// set callback functions for errors
        /// </summary>
        [DllImport("vrntzt_neat_c_dll_1_0.dll", EntryPoint = "set_error_stream")]
        public static extern void SetErrorStream(OutputCallback t_callback_f);
    }

    /// <summary>
    /// settings for neat evolution
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct NeatEvolutionSettings
    {
        public uint     PopulationSize;
        public uint     SpeciesCount;
        public float    InterspeciesMatingChance;
    }

    /// <summary>
    /// main evolution managing class
    /// </summary>
    public partial class NeatEvolutionManager
    {
        #region PUBLIC_METHODS
        /// <summary>
        /// creates a new instance of NEAT evolution manager
        /// </summary>
        /// <param name="t_input_num"></param>
        /// <param name="t_output_num"></param>
        /// <param name="t_settings"></param>
        public NeatEvolutionManager(uint t_input_num, uint t_output_num,
            NeatEvolutionSettings t_settings)
        {
            _handler = _newNeatEvolutionManager(t_input_num, t_output_num,
                t_settings);

            _populationSize = t_settings.PopulationSize;
        }

        /// <summary>
        /// finalizer
        /// </summary>
        ~NeatEvolutionManager()
        {
            _deleteNeatEvolutionManager(ref _handler);
        }

        /// <summary>
        /// save current population
        /// </summary>
        public void Save(string t_file)
        {
            _save(_handler, Encoding.ASCII.GetBytes(t_file));
        }

        /// <summary>
        /// load population
        /// </summary>
        public void Load(string t_file)
        {
            _load(_handler, Encoding.ASCII.GetBytes(t_file));
        }

        /// <summary>
        /// create a new random starting population and deletes the old one
        /// NOTE: calling this method invalidates all instances of SimplisticGenotype!
        /// </summary>
        public void CreateRandomPopulation()
        {
            _createRandomPopulation(_handler);
        }

        /// <summary>
        /// executes one generation worth of evolution
        /// all Genotypes should be rated a fitness before calling this method
        /// NOTE: calling this method invalidates all instances of SimplisticGenotype!
        /// </summary>
        public void EvolvePopulation()
        {
            _evolvePopulation(_handler);
        }

        /// <summary>
        /// returns all genotypes of the current generation
        /// </summary>
        /// <returns> array with size of population </returns>
        public SimplisticGenotype[] GetPopulation()
        {
            // two arrays: array of handlers and array of actual objects
            SimplisticGenotypeHandler[] population_handler =
                new SimplisticGenotypeHandler[_populationSize];
            SimplisticGenotype[] population =
                new SimplisticGenotype[_populationSize];

            _getPopulation(_handler, population_handler, _populationSize);

            // create SimplisticGenotype objects with handlers
            for (uint i = 0; i < _populationSize; ++i)
            {
                population[i] = new SimplisticGenotype(population_handler[i]);
            }

            return population;
        }

        #endregion PUBLIC_METHODS
    }

    /// <summary>
    /// phenotype representation of network
    /// used for evaluation
    /// </summary>
    public partial class SimplisticPhenotype : IDisposable
    {
        #region PUBLIC_METHODS

        /// <summary>
        /// internal constructor - called by SimplisticGenotype.decode()
        /// </summary>
        /// <param name="t_handler"></param>
        internal SimplisticPhenotype(SimplisticPhenotypeHandler t_handler)
        {
            _handler = t_handler;
        }

        ~SimplisticPhenotype()
        {
            Dispose(false);
        }

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        /// <summary>
        /// set input of neuronal network
        /// </summary>
        /// <param name="t_index"></param>
        /// <param name="t_value"></param>
        public void SetInput(ulong t_index, float t_value)
        {
            _setInput(_handler, t_index, t_value);
        }

        /// <summary>
        /// returns input of neuronal network
        /// </summary>
        /// <param name="t_index"></param>
        /// <returns></returns>
        public float GetInput(ulong t_index)
        {
            return _getInput(_handler, t_index);
        }

        /// <summary>
        /// returns output of neuronal network
        /// </summary>
        /// <param name="t_index"></param>
        /// <returns></returns>
        public float GetOutput(ulong t_index)
        {
            return _getOutput(_handler, t_index);
        }

        /// <summary>
        /// activates neuronal network
        /// </summary>
        /// <param name="t_iterations"></param>
        public void Activate(uint t_iterations)
        {
            _activate(_handler, t_iterations);
        }

        /// <summary>
        /// status of Phenotype
        /// </summary>
        /// <returns> bool flag </returns>
        public bool IsWorking()
        {
            return _isWorking(_handler);
        }

        /// <summary>
        /// resets outputs and internal status of neuronal network
        /// </summary>
        public void Reset()
        {
            _reset(_handler);
        }

        #endregion PUBLIC_METHODS
    }

    /// <summary>
    /// IMPORTANT: genotype gets invalid after evolvePopulation or createRandomPopulation
    /// are called!
    /// </summary>
    public partial class SimplisticGenotype
    {
        #region PUBLIC_METHODS

        internal SimplisticGenotype(SimplisticGenotypeHandler t_handler)
        {
            _handler = t_handler;
        }

        /// <summary>
        /// returns user-set fitness of Genotype
        /// </summary>
        /// <returns></returns>
        public float GetFitness()
        {
            return _getFitness(_handler);
        }

        /// <summary>
        /// sets fitness of Genotype
        /// fitness is the primary factor when evolving
        /// </summary>
        /// <param name="t_fitness"> float higher than or equal zero, higher is better </param>
        public void SetFitness(float t_fitness)
        {
            _setFitness(_handler, t_fitness);
        }

        /// <summary>
        /// decodes Genotype into Phenotype
        /// </summary>
        /// <returns> instance of SimplisticPhenotype 
        /// IMPORTANT: SimplisticGenotype is IDisposable and should ONLY be constructed
        /// with 'using' or try-finally </returns>
        public SimplisticPhenotype Decode()
        {
            SimplisticPhenotypeHandler phenotype_handler = _decode(_handler);

            return new SimplisticPhenotype(phenotype_handler);
        }

        #endregion PUBLIC_METHODS
    }
}
