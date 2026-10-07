# PQC Kyber com hashing caótico

Implementação em C com cabeçalhos compatíveis com C++ via `extern "C"` condicionado a `__cplusplus`. O núcleo `hash_options/chaotic_hash.c` permanece original.

## Compilar e testar

Requer compilador C, make e Python 3. Para os alvos NIST KAT, também são necessários os cabeçalhos OpenSSL.

```bash
make -C ref test
make -C ref check
```

`test_hashing` verifica vetores originais e a continuidade de extrações XOF fracionadas. Os bytes residuais ficam no adaptador, fora do núcleo matemático.

`test_rejection` gera um ciphertext com sementes fixas, inverte um bit por caso e exige o segredo de rejeição correto. `run_checks.py` executa todos os dez testes e retorna falha se qualquer um reprovar.

## Falhas conhecidas

As regressões de rejeição permanecem reprovadas: 65/6.144 alterações em Kyber512, 65/8.704 em Kyber768 e 65/12.544 em Kyber1024, para as sementes fixas do teste. Foram identificadas perdas de informação na normalização Q24 e nos truncamentos do mapa caótico. Nenhum caso foi excluído ou convertido em sucesso esperado.

Passar nos testes de encapsulação legítima não elimina essas falhas nem comprova a segurança da substituição das primitivas por hashing caótico. Esta atualização não inclui ROS2 e não altera a precisão numérica do núcleo.
