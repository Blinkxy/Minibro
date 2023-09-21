/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdouzi < mdouzi@student.1337.ma>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 05:00:49 by mdouzi            #+#    #+#             */
/*   Updated: 2023/09/21 05:33:01 by mdouzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


// void handle_prompt(int sig)
// {
//     (void)sig;
//     // gb_ex_st = 130;
//     printf("\n");
//     rl_replace_line("", 0);
//     rl_on_new_line();
//     rl_redisplay();
// }

// void	restore_prompt(int sig)
// {
// 	// gb_ex_st = 130;
// 	write(1, "\n", 1);
// 	rl_replace_line("", 0);
// 	rl_on_new_line();
// 	rl_redisplay();
// 	(void)sig;
// }

// void	ctrl_c(int sig)
// {
// 	// gb_ex_st = 130;
// 	write(1, "\n", 1);
// 	(void)sig;
// }

// void	back_slash(int sig)
// {
// 	// gb_ex_st = 131;
// 	printf("Quit (core dumped)\n");
// 	(void)sig;
// }

// void handle_signals(int mode, t_general sa)
// {
//     if(mode == 1)
//     {
//         signal(SIGINT, handle_prompt);
//         signal(SIGQUIT, SIG_IGN);
//     }
//     if(mode  == 2)
//     {
//         signal(SIGINT, ctrl_c);
// 		signal(SIGQUIT, back_slash);
// 	}
//     if(mode == 3)
//     {
//         printf("exit\n");
// 		exit(0);
//     }
// }

