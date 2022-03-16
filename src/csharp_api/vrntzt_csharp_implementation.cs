// ch 2022
// netstandard 2.0
// v1.0.0
// vrntzt
// c# api
// implementation for simplistic genotype/phenotype & corresponding evolution

// TODO: reduce IO callback handles to 4 instead of 5

using System;
using System.Runtime.InteropServices;
using System.Text;

namespace vrntzt.neat.csharp_api
{
    public static partial class IO
    {
        static GCHandle? _all_streams_handle = null;
        static GCHandle? _out_stream_handle = null;
        static GCHandle? _debug_stream_handle = null;
        static GCHandle? _warning_stream_handle = null;
        static GCHandle? _error_stream_handle = null;

        [DllImport("vrntzt_neat_c_dll_1_0.dll", EntryPoint = "set_all_streams")]
        public static extern void _setAllStreams(OutputCallback t_callback_f);

        [DllImport("vrntzt_neat_c_dll_1_0.dll", EntryPoint = "set_out_stream")]
        public static extern void _setOutStream(OutputCallback t_callback_f);

        [DllImport("vrntzt_neat_c_dll_1_0.dll", EntryPoint = "set_debug_stream")]
        public static extern void _setDebugStream(OutputCallback t_callback_f);

        [DllImport("vrntzt_neat_c_dll_1_0.dll", EntryPoint = "get_fset_warning_streamitness")]
        public static extern void _setWarningStream(OutputCallback t_callback_f);

        [DllImport("vrntzt_neat_c_dll_1_0.dll", EntryPoint = "set_error_stream")]
        public static extern void _setErrorStream(OutputCallback t_callback_f);
    }

    public partial class SimplisticPhenotype : IDisposable
    {
        #region NON_PUBLIC_METHODS

        protected virtual void Dispose(bool t_disposing)
        {
            if (_disposed)
            {
                return;
            }
            else
            {
                _deleteSimplisticPhenotype(ref _handler);
                _disposed = true;
            }
        }

        #endregion NON_PUBLIC_METHODS

        #region PRIVATE_PROPERTIES

        private SimplisticPhenotypeHandler _handler;
        private bool _disposed = false;

        #endregion PRIVATE_PROPERTIES

        #region PRIVATE_DLL_METHODS

        [DllImport("vrntzt_neat_c_dll_1_0.dll", EntryPoint = "delete_simplistic_phenotype")]
        private static extern void _deleteSimplisticPhenotype(
            ref SimplisticPhenotypeHandler t_wrapper);

        [DllImport("vrntzt_neat_c_dll_1_0.dll", EntryPoint = "set_input")]
        private static extern void _setInput(SimplisticPhenotypeHandler t_wrapper,
            ulong t_index, float t_value);

        [DllImport("vrntzt_neat_c_dll_1_0.dll", EntryPoint = "get_input")]
        private static extern float _getInput(SimplisticPhenotypeHandler t_wrapper,
            ulong t_index);

        [DllImport("vrntzt_neat_c_dll_1_0.dll", EntryPoint = "get_output")]
        private static extern float _getOutput(SimplisticPhenotypeHandler t_wrapper,
            ulong t_index);

        [DllImport("vrntzt_neat_c_dll_1_0.dll", EntryPoint = "activate")]
        private static extern void _activate(SimplisticPhenotypeHandler t_wrapper,
            uint t_iterations);

        [DllImport("vrntzt_neat_c_dll_1_0.dll", EntryPoint = "is_working")]
        private static extern bool _isWorking(SimplisticPhenotypeHandler t_wrapper);

        [DllImport("vrntzt_neat_c_dll_1_0.dll", EntryPoint = "reset")]
        private static extern void _reset(SimplisticPhenotypeHandler t_wrapper);

        #endregion PRIVATE_DLL_METHODS
    }

    // IMPORTANT: genotype gets invalid after evolvePopulation has been called!
    public partial class SimplisticGenotype
    {
        #region PRIVATE_PROPERTIES

        private SimplisticGenotypeHandler _handler;

        #endregion PRIVATE_PROPERTIES

        #region PRIVATE_DLL_METHODS

        [DllImport("vrntzt_neat_c_dll_1_0.dll", EntryPoint = "get_fitness")]
        private static extern float _getFitness(SimplisticGenotypeHandler t_wrapper);

        [DllImport("vrntzt_neat_c_dll_1_0.dll", EntryPoint = "set_fitness")]
        private static extern void _setFitness(SimplisticGenotypeHandler t_wrapper,
            float t_fitness);

        [DllImport("vrntzt_neat_c_dll_1_0.dll", EntryPoint = "decode")]
        private static extern SimplisticPhenotypeHandler _decode(
            SimplisticGenotypeHandler t_wrapper);

        #endregion PRIVATE_DLL_METHODS
    }

    public partial class NeatEvolutionManager
    {
        #region INTERNAL_HANDLER

        [StructLayout(LayoutKind.Sequential)]
        private struct NeatEvolutionManagerHandler
        {
            public UIntPtr Handler;
        }

        #endregion INTERNAL_HANDLER

        #region PRIVATE_PROPERTIES

        private NeatEvolutionManagerHandler _handler;
        private uint _populationSize;

        #endregion PRIVATE_PROPERTIES

        #region PRIVATE_DLL_METHODS

        [DllImport("vrntzt_neat_c_dll_1_0.dll", EntryPoint = "new_neat_evolution_manager")]
        private static extern NeatEvolutionManagerHandler _newNeatEvolutionManager(
           uint t_inputs, uint t_outputs, NeatEvolutionSettings t_settings);

        [DllImport("vrntzt_neat_c_dll_1_0.dll", EntryPoint = "delete_neat_evolution_manager")]
        private static extern void _deleteNeatEvolutionManager(
            ref NeatEvolutionManagerHandler t_manager);

        [DllImport("vrntzt_neat_c_dll_1_0.dll", EntryPoint = "create_random_population")]
        private static extern void _createRandomPopulation(
            NeatEvolutionManagerHandler t_manager);

        [DllImport("vrntzt_neat_c_dll_1_0.dll", EntryPoint = "save")]
        private static extern void _save(
            NeatEvolutionManagerHandler t_manager, byte[] t_file);

        [DllImport("vrntzt_neat_c_dll_1_0.dll", EntryPoint = "load")]
        private static extern void _load(
            NeatEvolutionManagerHandler t_manager, byte[] t_file);

        [DllImport("vrntzt_neat_c_dll_1_0.dll", EntryPoint = "evolve_population")]
        private static extern void _evolvePopulation(
            NeatEvolutionManagerHandler t_manager);

        [DllImport("vrntzt_neat_c_dll_1_0.dll", EntryPoint = "get_population")]
        private static extern void _getPopulation(
            NeatEvolutionManagerHandler t_wrapped_manager,
            [Out] SimplisticGenotypeHandler[] t_target, ulong t_target_size);

        #endregion PRIVATE_DLL_METHODS
    }

    #region INTERNAL_HANDLERS

    [StructLayout(LayoutKind.Sequential)]
    internal struct SimplisticGenotypeHandler
    {
        public UIntPtr Handler;
    }

    [StructLayout(LayoutKind.Sequential)]
    internal struct SimplisticPhenotypeHandler
    {
        public UIntPtr Handler;
    }

    #endregion INTERNAL_HANDLERS
}

