using System;
using System.Text;

namespace vrntzt.neat.csharp_api
{ 
    public static class NeatXor
    {
        public static float EvalXor(SimplisticGenotype t_genotype)
        {
            float fitness = 0;
            SimplisticPhenotype phenotype = t_genotype.Decode();

            try
            {
                fitness += 1 - _neatXor(phenotype, 0, 0);
                fitness += _neatXor(phenotype, 0, 1);
                fitness += _neatXor(phenotype, 1, 0);
                fitness += 1 - _neatXor(phenotype, 1, 1);
            }
            finally
            {
                phenotype.Dispose();
            }

            return fitness;
        }

        private static float _neatXor(SimplisticPhenotype t_phenotype,
            float t_in1, float t_in2)
        {
            t_phenotype.Reset();
            t_phenotype.SetInput(0, t_in1);
            t_phenotype.SetInput(1, t_in2);
            t_phenotype.Activate(10);

            return t_phenotype.GetOutput(0);
        }
    }

    public class Program
    {

        /// <summary>
        /// output callback function
        /// </summary>
        internal static void OutputCallbackFunction(StringBuilder t_string, uint t_len)
        {
            Console.WriteLine(t_string);
        }

        static void Main(string[] args)
        {
            // set output callback
            IO.SetAllStreams(OutputCallbackFunction);

            Random random = new Random();

            NeatEvolutionSettings settings = new NeatEvolutionSettings()
            {
                PopulationSize = 100,
                SpeciesCount = 10,
                InterspeciesMatingChance = 0.001f
            };

            NeatEvolutionManager manager = new NeatEvolutionManager(2, 1, settings);

            // starting population
            manager.CreateRandomPopulation();

            uint generation = 0;

            while (true)
            {
                Console.WriteLine($"\n============== Generation {++generation} ==============");

                // eval fitness
                foreach (var individual in manager.GetPopulation())
                {
                    individual.SetFitness(NeatXor.EvalXor(individual));
                }

                manager.EvolvePopulation();
            }
        }
    }
}
