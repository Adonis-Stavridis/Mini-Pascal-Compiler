#include "gencode.h"

void genData(SymbolTable *table)
{
	fprintf(yyout, "\t.data\n");
	fprintf(yyout, "st_true:\t.asciiz \"true\" \n");
	fprintf(yyout, "st_false:\t.asciiz \"false\" \n");
	fprintf(yyout, "st_newline:\t.asciiz \"\\n\"\n");
	fprintf(yyout, "st_error_div:\t.asciiz \"Exception: division by zero\"\n");
	fprintf(yyout, "st_error_range:\t.asciiz \"Segfault: access out of range\"\n");

	for (size_t i = 0; i < table->capacity; i++)
	{
		SymbolStack **stack = &table->data[i];
		if (stack)
		{
			SymbolNode *node = (*stack);
			while (node)
			{
				Symbol *symbol = node->symbol;
				switch (symbol->dataType)
				{
				case INT_TYPE:
				{
					int intVal = symbol->data.intVal;
					fprintf(yyout, "_%s:\t.word %d\n", symbol->name, intVal);
					break;
				}

				case BOOL_TYPE:
				{
					bool boolVal = symbol->data.boolVal;
					fprintf(yyout, "_%s:\t.byte %d\n", symbol->name, boolVal);
					break;
				}

				case STR_TYPE:
				{
					char *strVal = symbol->data.strVal;
					fprintf(yyout, "_%s:\t.asciiz %s\n", symbol->name, strVal);
					break;
				}

				case ARR_TYPE:
				{
					ArrayData *data = symbol->data.arrVal;
					int arraySize = 1;
					for (int i = 0; i < data->dims; i++)
					{
						arraySize *= data->max[i] - data->min[i] + 1;
					}

					switch (data->arrayType)
					{
					case INT_TYPE:
						fprintf(yyout, "_%s:\t.word", symbol->name);
						for (int i = 0; i < arraySize; i++)
						{
							fprintf(yyout, " 0");
						}
						fprintf(yyout, "\n");
						fprintf(yyout, "_%s_sz:\t.word %d\n", symbol->name, arraySize);
						break;

					case BOOL_TYPE:
						fprintf(yyout, "_%s:\t.byte", symbol->name);
						for (int i = 0; i < arraySize; i++)
						{
							fprintf(yyout, " 0");
						}
						fprintf(yyout, "\n");
						fprintf(yyout, "_%s_sz:\t.word %d\n", symbol->name, arraySize);
						break;

					default:
						typeError(data->arrayType, INT_TYPE);
						break;
					}
					break;
				}

				default:
					defaultError("Symbol table: entry of unknown type");
					break;
				}
				node = node->next;
			}
		}
	}

	fprintf(yyout, "\n");
}

void genQuad(Quad *quad)
{
	static int cnt = 0;
	static QuadStack *stack = NULL;

	switch (quad->op)
	{
	case ASS_OP:
	{
		switch (quad->arg1->dataType)
		{
		case INT_TYPE:
		{
			fprintf(yyout, "\tlw $t0 _%s\n", quad->arg1->name);
			fprintf(yyout, "\tsw $t0 _%s\n", quad->result->name);
			break;
		}

		case BOOL_TYPE:
		{
			fprintf(yyout, "\tlb $t0 _%s\n", quad->arg1->name);
			fprintf(yyout, "\tsb $t0 _%s\n", quad->result->name);
			break;
		}

		default:
			defaultError("ASS_OP: unknown type");
			break;
		}

		break;
	}

	case ADD_OP:
	{
		fprintf(yyout, "\tlw $t0 _%s\n", quad->arg1->name);
		fprintf(yyout, "\tlw $t1 _%s\n", quad->arg2->name);
		fprintf(yyout, "\tadd $t0 $t0 $t1\n");
		fprintf(yyout, "\tsw $t0 _%s\n", quad->result->name);
		break;
	}

	case SUB_OP:
	{
		if (quad->arg2)
		{
			fprintf(yyout, "\tlw $t0 _%s\n", quad->arg1->name);
			fprintf(yyout, "\tlw $t1 _%s\n", quad->arg2->name);
			fprintf(yyout, "\tsub $t0 $t0 $t1\n");
			fprintf(yyout, "\tsw $t0 _%s\n", quad->result->name);
		}
		else
		{
			fprintf(yyout, "\tlw $t0 _%s\n", quad->arg1->name);
			fprintf(yyout, "\tsub $t0 $zero $t0\n");
			fprintf(yyout, "\tsw $t0 _%s\n", quad->result->name);
		}
		break;
	}

	case MUL_OP:
	{
		fprintf(yyout, "\tlw $t0 _%s\n", quad->arg1->name);
		fprintf(yyout, "\tlw $t1 _%s\n", quad->arg2->name);
		fprintf(yyout, "\tmul $t0 $t0 $t1\n");
		fprintf(yyout, "\tsw $t0 _%s\n", quad->result->name);
		break;
	}

	case DIV_OP:
	{
		fprintf(yyout, "\tlw $t0 _%s\n", quad->arg1->name);
		fprintf(yyout, "\tlw $t1 _%s\n", quad->arg2->name);
		fprintf(yyout, "\tbne $t1 0 _L%d\n", cnt);
		fprintf(yyout, "\tla $a0 st_error_div\n");
		fprintf(yyout, "\tli $v0 4\n");
		fprintf(yyout, "\tsyscall\n");
		fprintf(yyout, "\tjal _write_newline\n");
		fprintf(yyout, "\tj exit\n");
		fprintf(yyout, "_L%d:\n", cnt++);
		fprintf(yyout, "\tdiv $t0 $t0 $t1\n");
		fprintf(yyout, "\tsw $t0 _%s\n", quad->result->name);
		break;
	}

	case POW_OP:
	{
		static bool implemented = false;

		fprintf(yyout, "\tlw $t0 _%s\n", quad->arg1->name);
		fprintf(yyout, "\tlw $t1 _%s\n", quad->arg2->name);
		fprintf(yyout, "\tjal _power\n");
		fprintf(yyout, "\tsw $t0 _%s\n", quad->result->name);

		if (!implemented)
		{
			fprintf(yyout, "\tj _power_continue\n");
			fprintf(yyout, "\n");
			fprintf(yyout, "_power:\n");
			fprintf(yyout, "\tmove $t2 $t0\n");
			fprintf(yyout, "\tbne $t0 0 _power_not_null\n");
			fprintf(yyout, "\tli $t0 0\n");
			fprintf(yyout, "\tj _power_end\n");
			fprintf(yyout, "_power_not_null:\n");
			fprintf(yyout, "\tbne $t1 0 _power_exp_not_null\n");
			fprintf(yyout, "\tli $t0 1\n");
			fprintf(yyout, "\tj _power_end\n");
			fprintf(yyout, "\t_power_exp_not_null:\n");
			fprintf(yyout, "\tbgt $t1 1 _power_loop\n");
			fprintf(yyout, "\tli $t0 0\n");
			fprintf(yyout, "\tj _power_end\n");
			fprintf(yyout, "_power_loop:\n");
			fprintf(yyout, "\tble $t1 1 _power_end\n");
			fprintf(yyout, "\tmul $t0 $t0 $t2\n");
			fprintf(yyout, "\tsub $t1 $t1 1\n");
			fprintf(yyout, "\tj _power_loop\n");
			fprintf(yyout, "_power_end:\n");
			fprintf(yyout, "\tjr $ra\n");
			fprintf(yyout, "\n");
			fprintf(yyout, "_power_continue:\n");
			implemented = true;
		}

		break;
	}

	case AND_OP:
	{
		fprintf(yyout, "\tlb $t0 _%s\n", quad->arg1->name);
		fprintf(yyout, "\tlb $t1 _%s\n", quad->arg2->name);
		fprintf(yyout, "\tand $t0 $t0 $t1\n");
		fprintf(yyout, "\tsb $t0 _%s\n", quad->result->name);
		break;
	}

	case OR_OP:
	{
		fprintf(yyout, "\tlb $t0 _%s\n", quad->arg1->name);
		fprintf(yyout, "\tlb $t1 _%s\n", quad->arg2->name);
		fprintf(yyout, "\tor $t0 $t0 $t1\n");
		fprintf(yyout, "\tsb $t0 _%s\n", quad->result->name);
		break;
	}

	case XOR_OP:
	{
		fprintf(yyout, "\tlb $t0 _%s\n", quad->arg1->name);
		fprintf(yyout, "\tlb $t1 _%s\n", quad->arg2->name);
		fprintf(yyout, "\txor $t0 $t0 $t1\n");
		fprintf(yyout, "\tsb $t0 _%s\n", quad->result->name);
		break;
	}

	case NOT_OP:
	{
		fprintf(yyout, "\tlb $t0 _%s\n", quad->arg1->name);
		fprintf(yyout, "\tbeq $t0 $zero _L%d\n", cnt);
		fprintf(yyout, "\tli $t0 0\n");
		fprintf(yyout, "\tj _L%d\n", cnt + 1);
		fprintf(yyout, "_L%d:\n", cnt++);
		fprintf(yyout, "\tli $t0 1\n");
		fprintf(yyout, "_L%d:\n", cnt++);
		fprintf(yyout, "\tsb $t0 _%s\n", quad->result->name);
		break;
	}

	case EQ_OP:
	{
		switch (quad->arg1->dataType)
		{
		case INT_TYPE:
			fprintf(yyout, "\tlw $t0 _%s\n", quad->arg1->name);
			break;

		case BOOL_TYPE:
			fprintf(yyout, "\tlb $t0 _%s\n", quad->arg1->name);
			break;

		default:
			defaultError("EQ_OP: unknown type");
			break;
		}

		switch (quad->arg2->dataType)
		{
		case INT_TYPE:
			fprintf(yyout, "\tlw $t1 _%s\n", quad->arg2->name);
			break;

		case BOOL_TYPE:
			fprintf(yyout, "\tlb $t1 _%s\n", quad->arg2->name);
			break;

		default:
			defaultError("EQ_OP: unknown type");
			break;
		}

		fprintf(yyout, "\tbne $t0 $t1 _L%d\n", cnt);
		fprintf(yyout, "\tli $t0 1\n");
		fprintf(yyout, "\tj _L%d\n", cnt + 1);
		fprintf(yyout, "_L%d:\n", cnt++);
		fprintf(yyout, "\tli $t0 0\n");
		fprintf(yyout, "_L%d:\n", cnt++);
		fprintf(yyout, "\tsb $t0 _%s\n", quad->result->name);
		break;
	}

	case DIFF_OP:
	{
		switch (quad->arg1->dataType)
		{
		case INT_TYPE:
			fprintf(yyout, "\tlw $t0 _%s\n", quad->arg1->name);
			break;

		case BOOL_TYPE:
			fprintf(yyout, "\tlb $t0 _%s\n", quad->arg1->name);
			break;

		default:
			defaultError("DIFF_OP: unknown type");
			break;
		}

		switch (quad->arg2->dataType)
		{
		case INT_TYPE:
			fprintf(yyout, "\tlw $t1 _%s\n", quad->arg2->name);
			break;

		case BOOL_TYPE:
			fprintf(yyout, "\tlb $t1 _%s\n", quad->arg2->name);
			break;

		default:
			defaultError("DIFF_OP: unknown type");
			break;
		}

		fprintf(yyout, "\tbeq $t0 $t1 _L%d\n", cnt);
		fprintf(yyout, "\tli $t0 1\n");
		fprintf(yyout, "\tj _L%d\n", cnt + 1);
		fprintf(yyout, "_L%d:\n", cnt++);
		fprintf(yyout, "\tli $t0 0\n");
		fprintf(yyout, "_L%d:\n", cnt++);
		fprintf(yyout, "\tsb $t0 _%s\n", quad->result->name);
		break;
	}

	case GTH_OP:
	{
		fprintf(yyout, "\tlw $t0 _%s\n", quad->arg1->name);
		fprintf(yyout, "\tlw $t1 _%s\n", quad->arg2->name);
		fprintf(yyout, "\tble $t0 $t1 _L%d\n", cnt);
		fprintf(yyout, "\tli $t0 1\n");
		fprintf(yyout, "\tj _L%d\n", cnt + 1);
		fprintf(yyout, "_L%d:\n", cnt++);
		fprintf(yyout, "\tli $t0 0\n");
		fprintf(yyout, "_L%d:\n", cnt++);
		fprintf(yyout, "\tsb $t0 _%s\n", quad->result->name);
		break;
	}

	case LTH_OP:
	{
		fprintf(yyout, "\tlw $t0 _%s\n", quad->arg1->name);
		fprintf(yyout, "\tlw $t1 _%s\n", quad->arg2->name);
		fprintf(yyout, "\tbge $t0 $t1 _L%d\n", cnt);
		fprintf(yyout, "\tli $t0 1\n");
		fprintf(yyout, "\tj _L%d\n", cnt + 1);
		fprintf(yyout, "_L%d:\n", cnt++);
		fprintf(yyout, "\tli $t0 0\n");
		fprintf(yyout, "_L%d:\n", cnt++);
		fprintf(yyout, "\tsb $t0 _%s\n", quad->result->name);
		break;
	}

	case GEQ_OP:
	{
		fprintf(yyout, "\tlw $t0 _%s\n", quad->arg1->name);
		fprintf(yyout, "\tlw $t1 _%s\n", quad->arg2->name);
		fprintf(yyout, "\tblt $t0 $t1 _L%d\n", cnt);
		fprintf(yyout, "\tli $t0 1\n");
		fprintf(yyout, "\tj _L%d\n", cnt + 1);
		fprintf(yyout, "_L%d:\n", cnt++);
		fprintf(yyout, "\tli $t0 0\n");
		fprintf(yyout, "_L%d:\n", cnt++);
		fprintf(yyout, "\tsb $t0 _%s\n", quad->result->name);
		break;
	}

	case LEQ_OP:
	{
		fprintf(yyout, "\tlw $t0 _%s\n", quad->arg1->name);
		fprintf(yyout, "\tlw $t1 _%s\n", quad->arg2->name);
		fprintf(yyout, "\tbgt $t0 $t1 _L%d\n", cnt);
		fprintf(yyout, "\tli $t0 1\n");
		fprintf(yyout, "\tj _L%d\n", cnt + 1);
		fprintf(yyout, "_L%d:\n", cnt++);
		fprintf(yyout, "\tli $t0 0\n");
		fprintf(yyout, "_L%d:\n", cnt++);
		fprintf(yyout, "\tsb $t0 _%s\n", quad->result->name);
		break;
	}

	case READ_OP:
	{
		switch (quad->result->dataType)
		{
		case INT_TYPE:
		{
			fprintf(yyout, "\tli $v0 5\n");
			fprintf(yyout, "\tsyscall\n");
			fprintf(yyout, "\tsw $v0 _%s\n", quad->result->name);
			break;
		}

		case BOOL_TYPE:
		{
			fprintf(yyout, "\tli $v0 5\n");
			fprintf(yyout, "\tsyscall\n");
			fprintf(yyout, "\tsb $v0 _%s\n", quad->result->name);
			break;
		}

		default:
			defaultError("READ_OP: unknown type");
			break;
		}

		break;
	}

	case WRTE_OP:
	{
		switch (quad->arg1->dataType)
		{
		case INT_TYPE:
		{
			static bool implemented = false;

			fprintf(yyout, "\tlw $t0 _%s\n", quad->arg1->name);
			fprintf(yyout, "\tjal _write_int\n");

			if (!implemented)
			{
				fprintf(yyout, "\tj _L%d\n", cnt);
				fprintf(yyout, "\n");
				fprintf(yyout, "_write_int:\n");
				fprintf(yyout, "\tli $v0 1\n");
				fprintf(yyout, "\tmove $a0 $t0\n");
				fprintf(yyout, "\tsyscall\n");
				fprintf(yyout, "\tsw $ra 0($sp)\n");
				fprintf(yyout, "\tjal _write_newline\n");
				fprintf(yyout, "\tlw $ra 0($sp)\n");
				fprintf(yyout, "\tjr $ra\n");
				fprintf(yyout, "\n");
				fprintf(yyout, "_L%d:\n", cnt++);
				implemented = true;
			}

			break;
		}

		case BOOL_TYPE:
		{
			static bool implemented = false;

			fprintf(yyout, "\tlb $t0 _%s\n", quad->arg1->name);
			fprintf(yyout, "\tjal _write_bool\n");

			if (!implemented)
			{
				fprintf(yyout, "\tj _L%d\n", cnt);
				fprintf(yyout, "\n");
				fprintf(yyout, "_write_bool:\n");
				fprintf(yyout, "\tli $v0 4\n");
				fprintf(yyout, "\tbeq $t0 0 _write_bool_false\n");
				fprintf(yyout, "\tla $a0 st_true\n");
				fprintf(yyout, "\tj _write_bool_end\n");
				fprintf(yyout, "_write_bool_false:\n");
				fprintf(yyout, "\tla $a0 st_false\n");
				fprintf(yyout, "_write_bool_end:\n");
				fprintf(yyout, "\tsyscall\n");
				fprintf(yyout, "\tsw $ra 0($sp)\n");
				fprintf(yyout, "\tjal _write_newline\n");
				fprintf(yyout, "\tlw $ra 0($sp)\n");
				fprintf(yyout, "\tjr $ra\n");
				fprintf(yyout, "\n");
				fprintf(yyout, "_L%d:\n", cnt++);
				implemented = true;
			}

			break;
		}

		case STR_TYPE:
		{
			static bool implemented = false;

			fprintf(yyout, "\tla $a0 _%s\n", quad->arg1->name);
			fprintf(yyout, "\tjal _write_str\n");

			if (!implemented)
			{
				fprintf(yyout, "\tj _L%d\n", cnt);
				fprintf(yyout, "\n");
				fprintf(yyout, "_write_str:\n");
				fprintf(yyout, "\tli $v0 4\n");
				fprintf(yyout, "\tsyscall\n");
				fprintf(yyout, "\tsw $ra 0($sp)\n");
				fprintf(yyout, "\tjal _write_newline\n");
				fprintf(yyout, "\tlw $ra 0($sp)\n");
				fprintf(yyout, "\tjr $ra\n");
				fprintf(yyout, "\n");
				fprintf(yyout, "_L%d:\n", cnt++);
				implemented = true;
			}
			break;
		}

		default:
			defaultError("WRTE_OP: unknown type");
			break;
		}

		static bool implemented = false;

		if (!implemented)
		{
			fprintf(yyout, "\tj _L%d\n", cnt);
			fprintf(yyout, "\n");
			fprintf(yyout, "_write_newline:\n");
			fprintf(yyout, "\tla $a0 st_newline\n");
			fprintf(yyout, "\tli $v0 4\n");
			fprintf(yyout, "\tsyscall\n");
			fprintf(yyout, "\tjr $ra\n");
			fprintf(yyout, "\n");
			fprintf(yyout, "_L%d:\n", cnt++);
			implemented = true;
		}

		break;
	}

	case IF_OP:
	{
		stack = addQuadStackNode(stack, cnt);
		fprintf(yyout, "\tlb $t0 _%s\n", quad->arg1->name);
		fprintf(yyout, "\tbeq $t0 0 _L%d\n", cnt++);
		break;
	}

	case ELSE_OP:
	{
		int top = topQuadStack(stack);
		stack = popQuadStack(stack);
		stack = addQuadStackNode(stack, cnt);
		fprintf(yyout, "\tj _L%d\n", cnt++);
		fprintf(yyout, "_L%d:\n", top);
		break;
	}

	case IF_END_OP:
	{
		int top = topQuadStack(stack);
		stack = popQuadStack(stack);
		fprintf(yyout, "_L%d:\n", top);
		break;
	}

	case EXPR_M_OP:
	{
		stack = addQuadStackNode(stack, cnt);
		fprintf(yyout, "_L%d:\n", cnt++);
		break;
	}

	case WHLE_OP:
	{
		stack = addQuadStackNode(stack, cnt);
		fprintf(yyout, "\tlb $t0 _%s\n", quad->arg1->name);
		fprintf(yyout, "\tbeq $t0 0 _L%d\n", cnt++);
		break;
	}

	case WHLE_END_OP:
	{
		int end = topQuadStack(stack);
		stack = popQuadStack(stack);
		int loop = topQuadStack(stack);
		stack = popQuadStack(stack);
		fprintf(yyout, "\tj _L%d\n", loop);
		fprintf(yyout, "_L%d:\n", end);
		break;
	}

	case ARR_ASS_OP:
	{
		int dims = quad->result->data.arrVal->dims;
		Symbol **args = (Symbol **)calloc(dims, sizeof(Symbol));
		if (!args)
		{
			compilerError(_FILE, _FUNCTION, _LINE);
		}

		SymbolListNode *list = quad->list;
		for (int i = dims - 1; list; list = list->next, i--)
		{
			args[i] = getSymbolData(list);
		}

		fprintf(yyout, "\tli $t0 0\n");

		int j;
		for (j = 0; j < dims - 1; j++)
		{
			fprintf(yyout, "\tlw $t1 _%s\n", args[j]->name);
			fprintf(yyout, "\tbge $t1 %d _L%d\n", quad->result->data.arrVal->min[j], cnt);
			fprintf(yyout, "\tla $a0 st_error_range\n");
			fprintf(yyout, "\tli $v0 4\n");
			fprintf(yyout, "\tsyscall\n");
			fprintf(yyout, "\tjal _write_newline\n");
			fprintf(yyout, "\tj exit\n");
			fprintf(yyout, "_L%d:\n", cnt++);
			fprintf(yyout, "\tble $t1 %d _L%d\n", quad->result->data.arrVal->max[j], cnt);
			fprintf(yyout, "\tla $a0 st_error_range\n");
			fprintf(yyout, "\tli $v0 4\n");
			fprintf(yyout, "\tsyscall\n");
			fprintf(yyout, "\tjal _write_newline\n");
			fprintf(yyout, "\tj exit\n");
			fprintf(yyout, "_L%d:\n", cnt++);
			fprintf(yyout, "\tli $t2 %d\n", quad->result->data.arrVal->min[j]);
			fprintf(yyout, "\tli $t3 %d\n", quad->result->data.arrVal->max[j + 1]);
			fprintf(yyout, "\tli $t4 %d\n", quad->result->data.arrVal->min[j + 1]);
			fprintf(yyout, "\tsub $t1 $t1 $t2\n");
			fprintf(yyout, "\tsub $t3 $t3 $t4\n");
			fprintf(yyout, "\tadd $t3 $t3 1\n");
			fprintf(yyout, "\tmul $t1 $t1 $t3\n");
			fprintf(yyout, "\tadd $t0 $t0 $t1\n");
		}

		fprintf(yyout, "\tlw $t1 _%s\n", args[j]->name);
		fprintf(yyout, "\tbge $t1 %d _L%d\n", quad->result->data.arrVal->min[j], cnt);
		fprintf(yyout, "\tla $a0 st_error_range\n");
		fprintf(yyout, "\tli $v0 4\n");
		fprintf(yyout, "\tsyscall\n");
		fprintf(yyout, "\tjal _write_newline\n");
		fprintf(yyout, "\tj exit\n");
		fprintf(yyout, "_L%d:\n", cnt++);
		fprintf(yyout, "\tble $t1 %d _L%d\n", quad->result->data.arrVal->max[j], cnt);
		fprintf(yyout, "\tla $a0 st_error_range\n");
		fprintf(yyout, "\tli $v0 4\n");
		fprintf(yyout, "\tsyscall\n");
		fprintf(yyout, "\tjal _write_newline\n");
		fprintf(yyout, "\tj exit\n");
		fprintf(yyout, "_L%d:\n", cnt++);
		fprintf(yyout, "\tli $t2 %d\n", quad->result->data.arrVal->min[j]);
		fprintf(yyout, "\tsub $t1 $t1 $t2\n");
		fprintf(yyout, "\tadd $t0 $t0 $t1\n");

		if (quad->result->data.arrVal->arrayType == INT_TYPE)
		{
			fprintf(yyout, "\tmul $t0 $t0 4\n");
		}
		fprintf(yyout, "\tla $t1 _%s\n", quad->result->name);
		fprintf(yyout, "\tadd $t1 $t1 $t0\n");

		switch (quad->arg1->dataType)
		{
		case INT_TYPE:
		{
			fprintf(yyout, "\tlw $t0 _%s\n", quad->arg1->name);
			fprintf(yyout, "\tsw $t0 0($t1)\n");
			break;
		}

		case BOOL_TYPE:
		{
			fprintf(yyout, "\tlb $t0 _%s\n", quad->arg1->name);
			fprintf(yyout, "\tsb $t0 0($t1)\n");
			break;
		}

		default:
			defaultError("ARR_W_OP: unknown type");
			break;
		}

		free(args);
		freeSymbolList(quad->list);

		break;
	}

	case ARR_R_OP:
	{
		int dims = quad->result->data.arrVal->dims;
		Symbol **args = (Symbol **)calloc(dims, sizeof(Symbol));
		if (!args)
		{
			compilerError(_FILE, _FUNCTION, _LINE);
		}

		SymbolListNode *list = quad->list;
		for (int i = dims - 1; list; list = list->next, i--)
		{
			args[i] = getSymbolData(list);
		}

		fprintf(yyout, "\tli $t0 0\n");

		int j;
		for (j = 0; j < dims - 1; j++)
		{
			fprintf(yyout, "\tlw $t1 _%s\n", args[j]->name);
			fprintf(yyout, "\tbge $t1 %d _L%d\n", quad->result->data.arrVal->min[j], cnt);
			fprintf(yyout, "\tla $a0 st_error_range\n");
			fprintf(yyout, "\tli $v0 4\n");
			fprintf(yyout, "\tsyscall\n");
			fprintf(yyout, "\tjal _write_newline\n");
			fprintf(yyout, "\tj exit\n");
			fprintf(yyout, "_L%d:\n", cnt++);
			fprintf(yyout, "\tble $t1 %d _L%d\n", quad->result->data.arrVal->max[j], cnt);
			fprintf(yyout, "\tla $a0 st_error_range\n");
			fprintf(yyout, "\tli $v0 4\n");
			fprintf(yyout, "\tsyscall\n");
			fprintf(yyout, "\tjal _write_newline\n");
			fprintf(yyout, "\tj exit\n");
			fprintf(yyout, "_L%d:\n", cnt++);
			fprintf(yyout, "\tli $t2 %d\n", quad->result->data.arrVal->min[j]);
			fprintf(yyout, "\tli $t3 %d\n", quad->result->data.arrVal->max[j + 1]);
			fprintf(yyout, "\tli $t4 %d\n", quad->result->data.arrVal->min[j + 1]);
			fprintf(yyout, "\tsub $t1 $t1 $t2\n");
			fprintf(yyout, "\tsub $t3 $t3 $t4\n");
			fprintf(yyout, "\tadd $t3 $t3 1\n");
			fprintf(yyout, "\tmul $t1 $t1 $t3\n");
			fprintf(yyout, "\tadd $t0 $t0 $t1\n");
		}

		fprintf(yyout, "\tlw $t1 _%s\n", args[j]->name);
		fprintf(yyout, "\tbge $t1 %d _L%d\n", quad->result->data.arrVal->min[j], cnt);
		fprintf(yyout, "\tla $a0 st_error_range\n");
		fprintf(yyout, "\tli $v0 4\n");
		fprintf(yyout, "\tsyscall\n");
		fprintf(yyout, "\tjal _write_newline\n");
		fprintf(yyout, "\tj exit\n");
		fprintf(yyout, "_L%d:\n", cnt++);
		fprintf(yyout, "\tble $t1 %d _L%d\n", quad->result->data.arrVal->max[j], cnt);
		fprintf(yyout, "\tla $a0 st_error_range\n");
		fprintf(yyout, "\tli $v0 4\n");
		fprintf(yyout, "\tsyscall\n");
		fprintf(yyout, "\tjal _write_newline\n");
		fprintf(yyout, "\tj exit\n");
		fprintf(yyout, "_L%d:\n", cnt++);
		fprintf(yyout, "\tli $t2 %d\n", quad->result->data.arrVal->min[j]);
		fprintf(yyout, "\tsub $t1 $t1 $t2\n");
		fprintf(yyout, "\tadd $t0 $t0 $t1\n");

		if (quad->result->data.arrVal->arrayType == INT_TYPE)
		{
			fprintf(yyout, "\tmul $t0 $t0 4\n");
		}
		fprintf(yyout, "\tla $t1 _%s\n", quad->result->name);
		fprintf(yyout, "\tadd $t1 $t1 $t0\n");

		switch (quad->result->data.arrVal->arrayType)
		{
		case INT_TYPE:
		{
			fprintf(yyout, "\tli $v0 5\n");
			fprintf(yyout, "\tsyscall\n");
			fprintf(yyout, "\tsw $v0 0($t1)\n");
			break;
		}

		case BOOL_TYPE:
		{
			fprintf(yyout, "\tli $v0 5\n");
			fprintf(yyout, "\tsyscall\n");
			fprintf(yyout, "\tsb $v0 0($t1)\n");
			break;
		}

		default:
			defaultError("ARR_R_OP: unknown type");
			break;
		}

		free(args);
		freeSymbolList(quad->list);

		break;
	}

	case ARR_OP:
	{
		int dims = quad->arg1->data.arrVal->dims;
		Symbol **args = (Symbol **)calloc(dims, sizeof(Symbol));
		if (!args)
		{
			compilerError(_FILE, _FUNCTION, _LINE);
		}

		SymbolListNode *list = quad->list;
		for (int i = dims - 1; list; list = list->next, i--)
		{
			args[i] = getSymbolData(list);
		}

		fprintf(yyout, "\tli $t0 0\n");

		int j;
		for (j = 0; j < dims - 1; j++)
		{
			fprintf(yyout, "\tlw $t1 _%s\n", args[j]->name);
			fprintf(yyout, "\tbge $t1 %d _L%d\n", quad->arg1->data.arrVal->min[j], cnt);
			fprintf(yyout, "\tla $a0 st_error_range\n");
			fprintf(yyout, "\tli $v0 4\n");
			fprintf(yyout, "\tsyscall\n");
			fprintf(yyout, "\tjal _write_newline\n");
			fprintf(yyout, "\tj exit\n");
			fprintf(yyout, "_L%d:\n", cnt++);
			fprintf(yyout, "\tble $t1 %d _L%d\n", quad->arg1->data.arrVal->max[j], cnt);
			fprintf(yyout, "\tla $a0 st_error_range\n");
			fprintf(yyout, "\tli $v0 4\n");
			fprintf(yyout, "\tsyscall\n");
			fprintf(yyout, "\tjal _write_newline\n");
			fprintf(yyout, "\tj exit\n");
			fprintf(yyout, "_L%d:\n", cnt++);
			fprintf(yyout, "\tli $t2 %d\n", quad->arg1->data.arrVal->min[j]);
			fprintf(yyout, "\tli $t3 %d\n", quad->arg1->data.arrVal->max[j + 1]);
			fprintf(yyout, "\tli $t4 %d\n", quad->arg1->data.arrVal->min[j + 1]);
			fprintf(yyout, "\tsub $t1 $t1 $t2\n");
			fprintf(yyout, "\tsub $t3 $t3 $t4\n");
			fprintf(yyout, "\tadd $t3 $t3 1\n");
			fprintf(yyout, "\tmul $t1 $t1 $t3\n");
			fprintf(yyout, "\tadd $t0 $t0 $t1\n");
		}

		fprintf(yyout, "\tlw $t1 _%s\n", args[j]->name);
		fprintf(yyout, "\tbge $t1 %d _L%d\n", quad->arg1->data.arrVal->min[j], cnt);
		fprintf(yyout, "\tla $a0 st_error_range\n");
		fprintf(yyout, "\tli $v0 4\n");
		fprintf(yyout, "\tsyscall\n");
		fprintf(yyout, "\tjal _write_newline\n");
		fprintf(yyout, "\tj exit\n");
		fprintf(yyout, "_L%d:\n", cnt++);
		fprintf(yyout, "\tble $t1 %d _L%d\n", quad->arg1->data.arrVal->max[j], cnt);
		fprintf(yyout, "\tla $a0 st_error_range\n");
		fprintf(yyout, "\tli $v0 4\n");
		fprintf(yyout, "\tsyscall\n");
		fprintf(yyout, "\tjal _write_newline\n");
		fprintf(yyout, "\tj exit\n");
		fprintf(yyout, "_L%d:\n", cnt++);
		fprintf(yyout, "\tli $t2 %d\n", quad->arg1->data.arrVal->min[j]);
		fprintf(yyout, "\tsub $t1 $t1 $t2\n");
		fprintf(yyout, "\tadd $t0 $t0 $t1\n");

		if (quad->arg1->data.arrVal->arrayType == INT_TYPE)
		{
			fprintf(yyout, "\tmul $t0 $t0 4\n");
		}
		fprintf(yyout, "\tla $t1 _%s\n", quad->arg1->name);
		fprintf(yyout, "\tadd $t1 $t1 $t0\n");

		switch (quad->arg1->data.arrVal->arrayType)
		{
		case INT_TYPE:
		{
			fprintf(yyout, "\tlw $t0 0($t1)\n");
			fprintf(yyout, "\tsw $t0 _%s\n", quad->result->name);
			break;
		}

		case BOOL_TYPE:
		{
			fprintf(yyout, "\tlb $t0 0($t1)\n");
			fprintf(yyout, "\tsb $t0 _%s\n", quad->result->name);
			break;
		}

		default:
			defaultError("ARR_OP: unknown type");
			break;
		}

		free(args);
		freeSymbolList(quad->list);

		break;
	}

	default:
		defaultError("Code generation: unknown operation");
		break;
	}
}

void genText(QuadList *quadList)
{
	fprintf(yyout, "\t.text\n");
	fprintf(yyout, "\t.globl main\n");
	fprintf(yyout, "main:\n");

	Quad *quad = quadList->start;
	while (quad)
	{
		genQuad(quad);
		quad = quad->next;
	}

	fprintf(yyout, "\n");
	fprintf(yyout, "exit:\n");
	fprintf(yyout, "\tli $v0 10\n");
	fprintf(yyout, "\tsyscall\n");
	fprintf(yyout, "\n");
}

void gencode(QuadList *quadList, SymbolTable *table)
{
	genData(table);
	genText(quadList);
}
