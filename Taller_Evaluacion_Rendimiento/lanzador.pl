#!/usr/bin/env perl
#######################################################################################
#* Fecha: 30 Octubre 2025
#* Autor: Xamuel Pérez Madrigal, Juan Felipe Gómez López, David Beltrán
#* Programa:
#*      lanzador.pl
#* Propósito:
#*      Lanzar múltiples programas de multiplicación de matrices
#*     se ejecutan los 4 experimentos 30 veces cada combinación de N y T
#*      y guardar las estadísticas en archivos .dat
######################################################################################*/
use strict;
use warnings;
use File::Path qw(make_path);

# Configuración
my @programas = ("./mmClasicaOpenMP", "./mmClasicaPosix", "./mmClasicaFork", "./mmFilasOpenMP"); # Ejecutables de los 4 programas de multiplicación de matrices
my @Ns        = (64, 128, 256, 512);   # Tamaños de matriz N (la matriz es NxN)
my @hilos     = (1, 2, 4, 8);          # Números de hilos/procesos a probar (T)
my $REPS      = 30;                    # Número de repeticiones de cada prueba
my $OUT_DIR   = "resultados";          # Carpeta donde se guardarán los resultados (.dat)
# --------------------------------

# Crea la carpeta de salida si no existe.
make_path($OUT_DIR) unless -d $OUT_DIR;

# Bucle principal: Itera sobre todos los tamaños de matriz (N)
for my $N (@Ns) {
  # Bucle anidado: Itera sobre el número de hilos (T)
  for my $t (@hilos) {
    # Bucle anidado: Itera sobre cada uno de los 4 programas
    for my $prog (@programas) {

      # 1. Generación del nombre del archivo de salida
      # Extrae el nombre del programa (ej: "mmClasicaOpenMP") quitando el "./"
      (my $tag = $prog) =~ s{^\./}{};
      # Crea la ruta del archivo de salida (ej: resultados/mmClasicaOpenMP_N64_T1.dat)
      my $outfile = "$OUT_DIR/${tag}_N${N}_T${t}.dat";
      # Abre el archivo para escritura (">"). Si falla, detiene el script.
      open my $FH, ">", $outfile or die "No puedo crear $outfile: $!";

      # 2. Bucle de Repeticiones
      # Repite la ejecución del programa $REPS (30) veces para obtener estadísticas
      for (my $r = 1; $r <= $REPS; $r++) {
        # Construye el comando a ejecutar: <programa> <N> <hilos>
        my $cmd = "$prog $N $t";

        # Capturar stdout + stderr
        # Ejecuta el comando, redirigiendo stderr (2) a stdout (1) para capturar todo en $out
        my $out = `$cmd 2>&1`;
        # Verifica el estado de salida del comando (si es diferente de 0, significa que falló)
        if ($? != 0) {
          die "Fallo ejecutando '$cmd' (status=$?):\n$out\n";
        }

        # 3. Extracción del tiempo de ejecución
        # Inicializa un array para guardar los números encontrados en la salida
        my @nums;
        # Itera sobre cada línea de la salida
        for my $line (split /\R/, $out) {
          # Busca líneas que contengan solo un número (entero o decimal)
          if ($line =~ /^\s*[+-]?\d+(?:\.\d+)?\s*$/) {
            $line =~ s/^\s+|\s+$//g;   # Limpia espacios al inicio y final del número
            push @nums, $line;        # Añade el número limpio al array @nums
          }
        }

        # 4. Selección del tiempo
        # Se asume que el tiempo medido es el valor numérico más relevante, usualmente el último o el más grande.
        my $time_us;
		# si el array no está vacio
        if (@nums) {
          # Filtra para quedarse solo con los números positivos 
          my @positivas = grep { $_ + 0 > 0 } @nums;
          if (@positivas) {
            # Por defecto, usa el último número positivo encontrado
            $time_us = $positivas[-1];
            # Busca el mayor valor positivo encontrado (el que se usará como tiempo)
            for my $v (@positivas) { $time_us = $v if $v + 0 > $time_us + 0; }
          } else {
            # Si no hay positivos, usa el último número encontrado, incluso si es negativo
            $time_us = $nums[-1];
          }
        }

        # Si no se encontró ningún número, lanza un error
        die "No pude extraer el tiempo de la salida:\n$out\n"
          unless defined $time_us && $time_us =~ /-?\d/;

        # Escribe el tiempo extraído en el archivo de salida
        print $FH "$time_us\n";
      }

      # 5. Finalización
      close $FH; # Cierra el archivo de resultados .dat
      print "[OK] Generado $outfile\n"; # Muestra mensaje de éxito
    }
  }
}

print "\nListo. Los .dat quedaron en '$OUT_DIR/'.\n" # Mensaje final
