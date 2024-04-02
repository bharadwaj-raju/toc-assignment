#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cfg.h"

cfg_t * cfg_init()
{
    cfg_t * cfg = malloc(sizeof(cfg_t));
    cfg->productions = set_init();
    cfg->start[0] = '\0';
    return cfg;
}

cfg_t * cfg_from_file(FILE * fp)
{
    cfg_t * cfg = cfg_init();
    char buf[1024];
    int lineno = 1;
    while (fgets(buf, 1024, fp)) {
        char * buf_trim = buf;
        while (isspace(buf_trim[0])) {
            buf_trim++;
        }
        if (buf_trim[0] == '#') {
            continue;
        }
        char variable[KEY_LEN];
        char substitution[KEY_LEN];
        if (sscanf(buf_trim, "%s -> %s", variable, substitution) == 2 && substitution[0] != '\'' && substitution[0] != '"') {
            cfg_add_production(cfg, variable, substitution);
        } else if (sscanf(buf_trim, "%s -> ''", variable) == 1 || sscanf(buf_trim, "%s -> \"\"", variable) == 1) {
            // lambda production
            cfg_add_production(cfg, variable, "");
        } else {
            fprintf(stderr, "could not parse line %d\n", lineno);
        }
    }
    cfg_set_start_variable(cfg, cfg->productions->data[0].key);
    return cfg;
}

void cfg_to_file(cfg_t * cfg, FILE * fp)
{
    for (size_t i = 0; i < cfg->productions->len; i++) {
        char * variable = cfg->productions->data[i].key;
        set_t * substitutions = cfg->productions->data[i].data;
        if (!substitutions) {
            continue;
        }
        for (size_t j = 0; j < substitutions->len; j++) {
            char * subst = substitutions->data[j].key;
            if (subst[0] == '\0') {
                fprintf(fp, "%s -> ''\n", variable);
            } else {
                fprintf(fp, "%s -> %s\n", variable, subst);
            }
        }
    }
}

void cfg_add_production(cfg_t * cfg, const char * variable, const char * substitution)
{
    set_t * for_variable = (set_t *)set_find(cfg->productions, variable);
    if (for_variable) {
        set_add_uniq(for_variable, substitution, &PRESENT);
    } else {
        for_variable = set_init();
        set_add_uniq(for_variable, substitution, &PRESENT);
        set_add_uniq(cfg->productions, variable, for_variable);
    }
}

void cfg_set_start_variable(cfg_t * cfg, const char * start_variable)
{
    strncpy(cfg->start, start_variable, KEY_LEN);
}

bool cfg_is_variable(cfg_t * cfg, char sym)
{
    return (set_find(cfg->productions, single_char_str(sym)) != NULL);
}

bool cfg_is_nullable(cfg_t * cfg, char sym)
{
    char * sym_str = single_char_str(sym);
    set_t * substitutions = set_find(cfg->productions, sym_str);
    if (!substitutions) {
        // terminal
        return false;
    }
    for (size_t i = 0; i < substitutions->len; i++) {
        char * subst = substitutions->data[i].key;
        if (strlen(subst) == 0) {
            return true;
        }
        bool all_nullable = true;
        for (size_t j = 0; j < strlen(subst); j++) {
            if (subst[j] == sym || !cfg_is_nullable(cfg, subst[j])) {
                all_nullable = false;
            }
        }
        // all symbols it produces are nullable, therefore it is also nullable
        if (all_nullable) {
            return true;
        }
    }
    return false;
}

void cfg_remove_nongenerating(cfg_t * cfg)
{
    (void)cfg;
}

set_t * cfg_reachable(cfg_t * cfg, char * start_sym)
{
    set_t * reachable_old = set_init();
    set_t * reachable_cur = set_init();
    set_add(reachable_cur, start_sym, &PRESENT);
    while (!set_equal(reachable_old, reachable_cur)) {
        set_clear(reachable_old);
        set_union_inplace(reachable_old, reachable_cur);
        for (size_t i = 0; i < reachable_old->len; i++) {
            char * variable = reachable_old->data[i].key;
            set_t * productions = set_find(cfg->productions, variable);
            if (!productions) {
                continue;
            }
            for (size_t j = 0; j < productions->len; j++) {
                char * prod = productions->data[j].key;
                for (size_t k = 0; k < strlen(prod); k++) {
                    set_add_uniq(reachable_cur, single_char_str(prod[k]), &PRESENT);
                }
            }
        }
    }
    set_free(reachable_old);
    return reachable_cur;
}

void cfg_remove_unreachable(cfg_t * cfg)
{
    set_t * reachable_vars = cfg_reachable(cfg, cfg->start);
    set_t * productions_reachable = set_init_with_capacity(cfg->productions->len);
    for (size_t i = 0; i < cfg->productions->len; i++) {
        char * variable = cfg->productions->data[i].key;
        if (set_find(reachable_vars, variable) != NULL) {
            set_add(productions_reachable, variable, cfg->productions->data[i].data);
        }
    }
    set_free(cfg->productions);
    cfg->productions = productions_reachable;
}

void cfg_purify(cfg_t * cfg)
{
    cfg_remove_unreachable(cfg);

    set_t * nullable_vars = set_init();
    for (size_t i = 0; i < cfg->productions->len; i++) {
        char * variable = cfg->productions->data[i].key;
        if (cfg_is_nullable(cfg, variable[0])) {
            set_add(nullable_vars, variable, &PRESENT);
        }
    }

    // printf("nullable: ");
    // set_print(nullable_vars);

    // replace rules containing nullable variables
    for (size_t i = 0; i < cfg->productions->len; i++) {
        char * variable = cfg->productions->data[i].key;
        set_t * substitutions = cfg->productions->data[i].data;
        if (!substitutions) {
            continue;
        }
        for (size_t j = 0; j < substitutions->len; j++) {
            char * subst = substitutions->data[j].key;
            size_t subst_len = strlen(subst);
            for (size_t k = 0; k < subst_len; k++) {
                char * sym = single_char_str(subst[k]);
                if (set_find(nullable_vars, sym) != NULL) {
                    // printf("nullable: %s found in %s -> %s, k = %lu, subst_len = %lu\n", sym, cfg->productions->data[i].key, subst, k, subst_len);
                    // add the production without this occurence of the nullable symbol
                    char * new_subst = malloc(sizeof(char) * KEY_LEN);
                    size_t new_subst_len = 0;
                    for (size_t l = 0; l < subst_len; l++) {
                        if (l != k) {
                            new_subst[new_subst_len++] = subst[l];
                        }
                    }
                    new_subst[new_subst_len] = '\0';
                    if (new_subst_len == 0 || strncmp(new_subst, variable, new_subst_len) == 0) {
                        continue;
                    }
                    // printf("    new subst: <%s> of len %lu\n", new_subst, new_subst_len);
                    set_add_uniq(substitutions, new_subst, &PRESENT);
                }
                free(sym);
            }
        }
    }

    // remove null productions
    for (size_t i = 0; i < cfg->productions->len; i++) {
        set_t * substitutions = cfg->productions->data[i].data;
        if (!substitutions) {
            continue;
        }
        set_t * new_substitutions = set_init_with_capacity(substitutions->len);
        for (size_t j = 0; j < substitutions->len; j++) {
            char * subst = substitutions->data[j].key;
            if (strlen(subst) != 0) {
                set_add_uniq(new_substitutions, subst, &PRESENT);
            }
        }
        set_free(substitutions);
        cfg->productions->data[i].data = new_substitutions;
    }

    // remove unit productions
    int unit_productions_found = 0;
    do {
        unit_productions_found = 0;
        for (size_t i = 0; i < cfg->productions->len; i++) {
            set_t * substitutions = cfg->productions->data[i].data;
            if (!substitutions) {
                continue;
            }
            set_t * new_substitutions = set_init_with_capacity(substitutions->len);
            for (size_t j = 0; j < substitutions->len; j++) {
                char * subst = substitutions->data[j].key;
                if (strlen(subst) == 1 && cfg_is_variable(cfg, subst[0])) {
                    unit_productions_found++;
                    // printf("found unit prod: %s -> %s\n", cfg->productions->data[i].key, subst);
                    char * sym_str = single_char_str(subst[0]);
                    set_union_inplace(new_substitutions, set_find(cfg->productions, sym_str));
                } else {
                    set_add_uniq(new_substitutions, subst, &PRESENT);
                }
            }
            set_free(substitutions);
            cfg->productions->data[i].data = new_substitutions;
        }
    } while (unit_productions_found != 0);

    // some variables may now be useless
    cfg_remove_unreachable(cfg);
}

bool cfg_is_chomsky_normalized(cfg_t * cfg)
{
    for (size_t i = 0; i < cfg->productions->len; i++) {
        set_t * substitutions = cfg->productions->data[i].data;
        if (!substitutions) {
            continue;
        }
        for (size_t j = 0; j < substitutions->len; j++) {
            char * subst = substitutions->data[j].key;
            if (strlen(subst) == 2) {
                if (!cfg_is_variable(cfg, subst[0]) || !cfg_is_variable(cfg, subst[1])) {
                    return false;
                }
            } else if (strlen(subst) == 1) {
                if (cfg_is_variable(cfg, subst[0])) {
                    return false;
                }
            } else {
                return false;
            }
        }
    }
    return true;
}