/**
 * @author sunny lee
 * @date 2018-10-27
 * @version eosio-v1.3
 */
#include <eosiolib/asset.hpp>
#include <eosiolib/eosio.hpp>
#include <string>
using namespace eosio;

class banana : public eosio::contract {
    public:
        using contract::contract;

        [[eosio::action]]
        void issue( account_name to, eosio::asset quantity, std::string memo ) {
            //require_auth( _self );
            add_balance( _self, to, quantity );
            record_actions( N(issue),  _self, to, quantity, memo );
        }

        [[eosio::action]]
        void transfer( account_name from, account_name to, eosio::asset quantity, std::string memo ){
            require_auth( from );
            require_recipient( from );
            require_recipient( to );

            sub_balance( from, quantity );
            add_balance( from, to, quantity );
            record_actions( N(transfer), from, to, quantity, memo );
        }

        [[eosio::action]]
        void getbalance( account_name owner ) {
            auto acnt = _accounts.get( owner );
            auto acnt_obj = name{acnt.owner};
            std::string acnt_str = acnt_obj.to_string();
            print(acnt_str, ": ", acnt.balance);
            print(acnt_str, ": ", acnt.balance);
        }


    private:

        struct [[eosio::table]] account {
            account_name    owner;
            eosio::asset    balance;

            uint64_t primary_key()const { return owner; }
        };

        struct [[eosio::table]] action_record {
            uint64_t        id;
            uint64_t        action_type;
            account_name    from;
            account_name    to;
            eosio::asset    balance;
            std::string     memo;

            uint64_t primary_key()const { return id; }
        };

        eosio::multi_index<N(accounts), account> _accounts;
        eosio::multi_index<N(actions), action_record> _act_rcrd;


        void add_balance( account_name owner, account_name to, eosio::asset quantity ) {
            auto toitr = _accounts.find( to );
            if( toitr == _accounts.end() ) {
                _accounts.emplace( owner, [&]( auto& a ){
                    a.owner = to;
                    a.balance = quantity;
                });
            } else {
                _accounts.modify( toitr, 0, [&]( auto& a ){
                    a.balance += quantity;        
                });
            }
        }

        void sub_balance( account_name owner, eosio::asset quantity ){
            const auto& from_account = _accounts.get( owner );
            _accounts.modify( from_account, owner, [&]( auto& a ){
                a.balance -= quantity;
            });
        }

        void record_actions( uint64_t action_type, account_name from, account_name to, eosio::asset quantity, std::string memo ){
            _act_rcrd.emplace( _self, [&]( auto& a ){
                a.id = _act_rcrd.available_primary_key();
                a.from = from;
                a.to = to;
                a.balance = quantity;
                a.action_type = action_type;
                a.memo = memo;
            });
        }
};

EOSIO_DISPATCH( eosio::token, (issue)(transfer)(getbalance) )

