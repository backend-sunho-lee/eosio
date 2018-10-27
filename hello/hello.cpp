/**
  * @authore sunny lee
  * @date 2018-10-27
  * @version eosio-v1.3
  * eosio document 1.3, Hello World! 따라하기
  */
#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>

using namespace eosio;

class hello : public contract {
    public:
        using contract::contract;

        [[eosio::action]]
        void hi( name user ) {
            require_auth(user);
            print( "Hello, ", name{user});
        };
};
EOSIO_ABI( hello, (hi))

