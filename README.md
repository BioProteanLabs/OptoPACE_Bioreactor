# OptoPACE_Bioreactor

The OptoPACE Bioreactor: An Accessible Turbidostat System with Phage-Assisted Continuous Evolution Capability and Optogenetic Controls
Authors
1. Crane, Ryan D.* 
2. Isaacs, William T.*
3. Ijaz, Manal*
4. Michelle Kim
5. Nicole Salazar
6. Brianna Botello
7. Flores-Mora, Fernando E.
8. Bartelle, Benjamin B.

*co-first

## Abstract:
Continuous culture systems are essential for synthetic biology techniques like Directed Evolution (DE) and Phage-Assisted Continuous Evolution (PACE), which require metabolically active bacterial cultures for extended periods. Turbidostats, which maintain cell density using real-time optical density (OD) feedback, offer ideal conditions for inducing gene expression during mid-log phase, which is the optimal growth stage for synthetic circuit activity. However, current PACE-compatible bioreactors are either cost-prohibitive or require significant technical expertise. Here, we present the OptoPACE Bioreactor (OPB), a low-cost, user-friendly turbidostat system optimized for PACE and optogenetic control. The OPB uses a 650 nm laser and photoresistor to measure turbidity, an Arduino microcontroller for feedback control, and NeoPixel LEDs to enable multispectral optogenetic stimulation. Designed with 3D-printed components and off-the-shelf electronics, the system operates with a single peristaltic pump and uses the siphon effect to simplify fluidics. A custom motor control shield and an LCD-rotary encoder interface enable code-free operation. The OPB supports dual bioreactors connected unidirectionally, ensuring continuous culture while maintaining precise control over optical inputs and nutrient flow. The entire system can be built for under $200, significantly lowering the barrier to entry for labs interested in continuous evolution. By combining affordability, modularity, and ease of use, the OptoPACE Bioreactor provides a robust platform for protein engineering and synthetic biology, enabling broader adoption of advanced continuous evolution methods like PACE.

<img src="https://raw.githubusercontent.com/BioProteanLabs/OptoPACE_Bioreactor/main/Images/OpenSystem.png" alt="Assembly Step 1" width="500">


## Metadata Overview:
Main design files: https://github.com/BioProteanLabs/OptoPACE_Bioreactor/tree/main
Target group: undergraduate researchers, under-resourced laboratories in synthetic biology, students interested in biological hardware
Skills required: 3d printing - easy; software implementation - easy; physical assembly - intermediate; electrical assembly - advanced



Continuous bacterial culture bioreactors are widely employed in biological research, with recent applications in the directed evolution (DE) of biomolecules [1, 3]. In a typical DE experiment, bioreactors  maintain a population of microorganisms in a metabolically active state to support growth and expression of synthetic biological programs. For methods like phage assisted continuous evolution (PACE), two tandem bioreactors are needed: one for growing a substrate culture that feeds into a second “lagoon” to support bacteriophages actively evolving. 
For PACE, maintaining the substrate culture at a steady state gives consistent conditions for DE, requiring either a chemostat or turbidostat control system. Chemostat bioreactors are simpler in that they supply a steady state of nutrients, while a turbidostat makes continual adjustments based on feedback—namely, culture density or turbidity [8, 9, 17]. Turbidostats allow for more optimal and customizable culture conditions but require continual monitoring and control hardware for adjustments based on feedback.
Turbidity is typically measured by optical density (OD), which reflects both light absorption by molecules in the culture, such as pigments, and light scattering from materials with different refractive indexes, typically cell membrane lipids. OD serves as a real-time quantitative assay of cell concentration, allowing researchers to infer the growth stage of the bacterial population. In a bacterial culture, OD measurements typically begin with exponential growth, reach mid-log phase, and eventually flatten out into “death phase” as resources become limited. For the production of recombinant proteins at experimental and industrial scales, mid-log phase is considered the optimal state for inducing protein expression or a synthetic biological program. In a turbidostat bioreactor, continuous monitoring of bacterial growth allows users to identify the mid-log phase, input nutrients to maintain this optimal point, and induce expression of biological circuits.
Induction of biological circuits presents another layer of control required in a bioreactor. Historically, this has meant adding specific sugars or their chemical analog to regulate bacterial operons to initiate expression [2, 16]. This requires an additional chemostat input for the induction agent. More recently, optogenetic circuits—based on biology from plants and cyanobacteria—allow for the induction of gene expression using light. Current developments in synthetic biology have even enabled wavelength-specific optogenetic circuits. With stable culture conditions and multiple biological control inputs, bioreactors could control several genetic circuits simultaneously [4, 10, 11, 14].
Despite their potential, experimental-scale turbidostat bioreactors are not commonly available. The literature indicates that establishing a functional PACE workflow often requires years of effort due to the dual challenges of designing effective synthetic biological circuits and building reliable hardware to support culture [12, 13]. Additional obstacles—such as extensive setup time and the need for frequent maintenance due to microbial biofilm accumulation on culture surfaces—make developing an automated PACE workflow even more difficult to implement.

**The OptoPACE Bioreactor is designed for optogenetic control of synthetic biological circuits.**
<img src="https://raw.githubusercontent.com/BioProteanLabs/OptoPACE_Bioreactor/main/Images/OPB_TurbidostatAndLagoon.png" alt="Assembly Step 1" width="500">


**The OptoPACE Bioreactor uses the siphon effect to minimize the number of pumps needed.**


<img src="https://raw.githubusercontent.com/BioProteanLabs/OptoPACE_Bioreactor/main/Images/SiphonSystem.png" alt="Assembly Step 1" width="500">


**The OptoPACE Bioreactor is controlled by a single Arduino microcontroller and a custom designed motor control shield. It uses a 650nm laser and a photoresistor to measure turbidity, and NeoPixel LEDs for multispectral control.**

<img src="https://raw.githubusercontent.com/BioProteanLabs/OptoPACE_Bioreactor/main/Images/System_Overview.png" alt="Assembly Step 1" width="500">

**The OptoPACE Bioreactor uses an LCD screen and rotary encoder as an interface.**
<img src="https://raw.githubusercontent.com/BioProteanLabs/OptoPACE_Bioreactor/main/Images/OPB_UIpseudocode.png" alt="Assembly Step 1" width="500">


**Turbidity measurements from a 20 hour continuous run.**
<img src="https://raw.githubusercontent.com/BioProteanLabs/OptoPACE_Bioreactor/main/Images/StressTest.jpeg" alt="Assembly Step 1" width="500">


 Early measurements required an hour to stabilize before giving consistent readings. Turbidity set point was 650, which was achieved after 9 hours of culture. The turbidostat was maintained for the next 4 hours before the media bottle ran out, giving high noise readings as air bubbles were introduced from the empty line.


## References
[1]	K. M. Esvelt, J. C. Carlson, and D. R. Liu, “A System for the Continuous Directed Evolution of Biomolecules,” Nature, vol. 472, no. 7344, pp. 499–503, Apr. 2011, doi: 10.1038/nature09929. 
[2]	A. H. Badran and D. R. Liu, “In vivo continuous directed evolution,” Current Opinion in Chemical Biology, vol. 24, pp. 1–10, Feb. 2015, doi: 10.1016/j.cbpa.2014.09.040. 
[3]	A. Novick and L. Szilard, “Description of the Chemostat,” Science, vol. 112, no. 2920, pp. 715–716, Dec. 1950, doi: 10.1126/science.112.2920.715. 
[4]	C. N. Takahashi, A. W. Miller, F. Ekness, M. J. Dunham, and E. Klavins, “A Low Cost, Customizable Turbidostat for Use in Synthetic Circuit Characterization,” ACS Synth Biol, vol. 4, no. 1, pp. 32–38, Jan. 2015, doi: 10.1021/sb500165g. 
[5]	A. M. McGeachy, Z. A. Meacham, and N. T. Ingolia, “An Accessible Continuous-Culture Turbidostat for Pooled Analysis of Complex Libraries,” ACS Synth. Biol., vol. 8, no. 4, pp. 844–856, Apr. 2019, doi: 10.1021/acssynbio.8b00529. 
[6]	J. De Baets, B. De Paepe, and M. De Mey, “Delaying production with prokaryotic inducible expression systems,” Microb Cell Fact, vol. 23, p. 249, Sept. 2024, doi: 10.1186/s12934-024-02523-w. 
[7]	T. Sanganeria and B. Bordoni, “Genetics, Inducible Operon,” in StatPearls, Treasure Island (FL): StatPearls Publishing, 2025. Accessed: May 28, 2025. [Online]. Available: http://www.ncbi.nlm.nih.gov/books/NBK564361/ 
[8]	R. Ohlendorf, R. R. Vidavski, A. Eldar, K. Moffat, and A. Möglich, “From Dusk till Dawn: One-Plasmid Systems for Light-Regulated Gene Expression,” Journal of Molecular Biology, vol. 416, no. 4, pp. 534–542, Mar. 2012, doi: 10.1016/j.jmb.2012.01.001. 
[9]	E. J. Olson, L. A. Hartsough, B. P. Landry, R. Shroff, and J. J. Tabor, “Characterizing bacterial gene circuit dynamics with optically programmed gene expression signals,” Nat Methods, vol. 11, no. 4, pp. 449–455, Apr. 2014, doi: 10.1038/nmeth.2884. 
[10]	P. Ramakrishnan and J. J. Tabor, “Repurposing Synechocystis PCC6803 UirS–UirR as a UV-Violet/Green Photoreversible Transcriptional Regulatory Tool in E. coli,” ACS Synth. Biol., vol. 5, no. 7, pp. 733–740, July 2016, doi: 10.1021/acssynbio.6b00068. 
[11]	L. Forbes, M. Papanatsiou, A. Palombo, J. M. Christie, and A. Amtmann, “Optogenetic control of gene expression in the cyanobacterium Synechococcus sp. PCC 7002,” Front. Bioeng. Biotechnol., vol. 12, Jan. 2025, doi: 10.3389/fbioe.2024.1529022. 
[12]	S. C. Popa, I. Inamoto, B. W. Thuronyi, and J. A. Shin, “Phage Assisted Continuous Evolution (PACE): A How-to Guide for Directed Evolution,” Nov. 2019, Accessed: Mar. 09, 2020. [Online]. Available: https://www.preprints.org/manuscript/201911.0040/v1 
[13]	S. C. Popa, I. Inamoto, B. W. Thuronyi, and J. A. Shin, “Phage-Assisted Continuous Evolution (PACE): A Guide Focused on Evolving Protein-DNA Interactions,” ACS Omega, vol. 5, no. 42, pp. 26957–26966, Oct. 2020, doi: 10.1021/acsomega.0c03508. 
[14]	U. Riek, R. Tuerk, T. Wallimann, U. Schlattner, and D. Neumann, “An Automated Home-Built Low-Cost Fermenter Suitable for Large-Scale Bacterial Expression of Proteins in Escherichia Coli,” BioTechniques, vol. 45, no. 2, pp. 187–189, Aug. 2008, doi: 10.2144/000112830. 
[15]	D. Matteau, V. Baby, S. Pelletier, and S. Rodrigue, “A Small-Volume, Low-Cost, and Versatile Continuous Culture Device,” PLOS ONE, vol. 10, no. 7, p. e0133384, July 2015, doi: 10.1371/journal.pone.0133384. 
[16]	B. G. Wong, C. P. Mancuso, S. Kiriakov, C. J. Bashor, and A. S. Khalil, “Precise, automated control of conditions for high-throughput growth of yeast and bacteria with eVOLVER,” Nat Biotechnol, vol. 36, no. 7, pp. 614–623, Aug. 2018, doi: 10.1038/nbt.4151. 
[17]	Z. J. Heins, C. P. Mancuso, S. Kiriakov, B. G. Wong, C. J. Bashor, and A. S. Khalil, “Designing Automated, High-throughput, Continuous Cell Growth Experiments Using eVOLVER,” J Vis Exp, no. 147, May 2019, doi: 10.3791/59652. 
[18]	V. Gopalakrishnan et al., “A low-cost, open-source evolutionary bioreactor and its educational use,” Sept. 01, 2022, bioRxiv. doi: 10.1101/729434. 

Licenses
Article: CC-BY 4.0. See: https://creativecommons.org/share-your-work/cclicenses/ 
Hardware: CERN-OHL-P Permissive. See: https://cern-ohl.web.cern.ch 
Software: MIT-0. See: https://opensource.org/license/mit-0